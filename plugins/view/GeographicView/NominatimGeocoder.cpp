/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <tulip/TlpQtTools.h>
#include <tulip/YajlFacade.h>

#include "NominatimGeocoder.h"

using namespace std;
using namespace tlp;

class NominatimResultsParser : public YajlParseFacade {

public:
  void parseString(string &value) override {
    if (_currentKey == "display_name") {
      addresses.push_back(value);
    } else if (_currentKey == "lat") {
      _lat = stod(value);
    } else if (_currentKey == "lon") {
      _lng = stod(value);
      latLngs.emplace_back(_lat, _lng);
    }
  }

  void parseMapKey(string &value) override {
    _currentKey = value;
  }

  vector<string> addresses;
  vector<pair<double, double>> latLngs;

private:
  string _currentKey;
  double _lat;
  double _lng;
};

NominatimGeocoder::NominatimGeocoder() {
  _networkAccessManager = new QNetworkAccessManager();
}

NominatimGeocoder::~NominatimGeocoder() {
  delete _networkAccessManager;
}

vector<NominatimGeocoderResult> NominatimGeocoder::getLatLngForAddress(const string &address) {

  QUrl nominatimSearchUrl;
  nominatimSearchUrl.setScheme("https");
  nominatimSearchUrl.setHost("nominatim.openstreetmap.org");
  nominatimSearchUrl.setPath("/search");
  nominatimSearchUrl.setQuery("q=" + tlpStringToQString(address) +
                              "&format=json&dedupe=1&limit=20");

  QNetworkRequest request;
  request.setUrl(nominatimSearchUrl);

  QNetworkReply *reply = _networkAccessManager->get(request);
  QEventLoop loop;
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();
  QByteArray jsonData = reply->readAll();

  NominatimResultsParser nominatimParser;
  nominatimParser.parse(reinterpret_cast<const unsigned char *>(jsonData.constData()),
                        jsonData.size());

  unsigned int nbResults = nominatimParser.addresses.size();

  vector<NominatimGeocoderResult> ret;

  for (unsigned int i = 0; i < nbResults; ++i) {
    NominatimGeocoderResult result;
    result.address = nominatimParser.addresses[i];
    result.latLng = nominatimParser.latLngs[i];
    ret.push_back(result);
  }

  return ret;
}
