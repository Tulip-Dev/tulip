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

#ifndef NOMINATIM_GEOCODER_H
#define NOMINATIM_GEOCODER_H

#include <string>
#include <vector>

class QNetworkAccessManager;

namespace tlp {

struct NominatimGeocoderResult {
  std::string address;
  std::pair<double, double> latLng;
};

class NominatimGeocoder {

public:
  NominatimGeocoder();
  ~NominatimGeocoder();

  std::vector<NominatimGeocoderResult> getLatLngForAddress(const std::string &address);

private:
  QNetworkAccessManager *_networkAccessManager;
};

} // namespace tlp

#endif // NOMINATIM_GEOCODER_H
