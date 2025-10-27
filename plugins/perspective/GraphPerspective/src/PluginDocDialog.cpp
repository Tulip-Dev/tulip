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
#include <tulip/TlpQtTools.h>
#include "PluginDocDialog.h"

void PluginDocDialog::showDoc(QWidget *parent, QString pluginName, QString pluginHelp,
                              tlp::ParameterListModel *model) {
  QString doc("<head><style type\"text/css\">a { color: " HTML_LINK_COLOR " }</style></head>");
  doc += pluginHelp;

  auto nbParams = model->rowCount();
  if (nbParams) {
    doc.append("<br/><br/>The parameters are:<ul>");
    for (int i = 0; i < nbParams; ++i) {
      auto name = model->getParameterName(i);
      bool mandatory = model->isMandatory(i);
      auto pos = name.indexOf("::");
      if (pos != -1)
        name = name.mid(pos + 2);
      doc.append(QString("<br/><b>-&nbsp;&quot;%1&quot;</b>%2%3")
                     .arg(name)
                     .arg(mandatory ? "&nbsp;<i>[mandatory]</i>" : "")
                     .arg(model->getParameterHelp(i)));
    }
  }

  // show doc in a simple dialog using a scrolling QTextEdit
  PluginDocDialog pluginDocDialog(parent);
  pluginDocDialog.ui->pluginDocText->setReadOnly(true);
  pluginDocDialog.ui->pluginDocText->setHtml(doc);
  pluginDocDialog.setWindowTitle(pluginName + " documentation");
  pluginDocDialog.setModal(true);
  pluginDocDialog.exec();
}
