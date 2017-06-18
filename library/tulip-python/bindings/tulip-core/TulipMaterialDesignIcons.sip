/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

namespace tlp {

class TulipMaterialDesignIcons {

%TypeHeaderCode
#include <tulip/TulipMaterialDesignIcons.h>
%End

%Docstring

.. versionadded:: 5.0

Helper class for the configuration of a Material Design Icon glyph.

`Material Design Icons <https://materialdesignicons.com/>`_ : a growing icon collection allows designers and developers
targeting various platforms to download icons in the format, color and size they need for any project.

A glyph has been added to Tulip enabling to use these great icons
as nodes and edges extremities shapes.

That class offers utility functions and supported icons names constants.

To set a node glyph as a Material Design icon, the Icon glyph must be associated
to the node through the modification of the "viewShape" integer property attached to the graph.
The name of the icon to use must then be set in the "viewIcon" string property.
As an example, the following code snippet activates the Icon glyph for all nodes
and sets the "account" icon (:mdicon:`account`)::

  # graph is an instance of a tlp.Graph object
  viewShape = graph.getIntegerProperty("viewShape")
  viewIcon = graph.getStringProperty("viewIcon")

  # sets the Icon glyph on all nodes
  viewShape.setAllNodeValue(tlp.NodeShape.Icon)
  # sets the "account" glyph for all nodes
  viewIcon.setAllNodeValue(tlp.TulipMaterialDesignIcons.Account);

%End

%If (NO_LIMIT_DOC)
%Docstring
The following string constants representing the supported icons are available:

  * :const:`tlp.TulipMaterialDesignIcons.AccessPoint` (:mdicon:`access-point`)
  * :const:`tlp.TulipMaterialDesignIcons.AccessPointNetwork` (:mdicon:`access-point-network`)
  * :const:`tlp.TulipMaterialDesignIcons.Account` (:mdicon:`account`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountAlert` (:mdicon:`account-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountBox` (:mdicon:`account-box`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountBoxOutline` (:mdicon:`account-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountCardDetails` (:mdicon:`account-card-details`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountCheck` (:mdicon:`account-check`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountCircle` (:mdicon:`account-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountConvert` (:mdicon:`account-convert`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountEdit` (:mdicon:`account-edit`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountKey` (:mdicon:`account-key`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountLocation` (:mdicon:`account-location`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountMinus` (:mdicon:`account-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountMultiple` (:mdicon:`account-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountMultipleMinus` (:mdicon:`account-multiple-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountMultipleOutline` (:mdicon:`account-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountMultiplePlus` (:mdicon:`account-multiple-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountNetwork` (:mdicon:`account-network`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountOff` (:mdicon:`account-off`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountOutline` (:mdicon:`account-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountPlus` (:mdicon:`account-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountRemove` (:mdicon:`account-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountSearch` (:mdicon:`account-search`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountSettings` (:mdicon:`account-settings`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountSettingsVariant` (:mdicon:`account-settings-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountStar` (:mdicon:`account-star`)
  * :const:`tlp.TulipMaterialDesignIcons.AccountSwitch` (:mdicon:`account-switch`)
  * :const:`tlp.TulipMaterialDesignIcons.Adjust` (:mdicon:`adjust`)
  * :const:`tlp.TulipMaterialDesignIcons.AirConditioner` (:mdicon:`air-conditioner`)
  * :const:`tlp.TulipMaterialDesignIcons.Airballoon` (:mdicon:`airballoon`)
  * :const:`tlp.TulipMaterialDesignIcons.Airplane` (:mdicon:`airplane`)
  * :const:`tlp.TulipMaterialDesignIcons.AirplaneLanding` (:mdicon:`airplane-landing`)
  * :const:`tlp.TulipMaterialDesignIcons.AirplaneOff` (:mdicon:`airplane-off`)
  * :const:`tlp.TulipMaterialDesignIcons.AirplaneTakeoff` (:mdicon:`airplane-takeoff`)
  * :const:`tlp.TulipMaterialDesignIcons.Airplay` (:mdicon:`airplay`)
  * :const:`tlp.TulipMaterialDesignIcons.Alarm` (:mdicon:`alarm`)
  * :const:`tlp.TulipMaterialDesignIcons.AlarmCheck` (:mdicon:`alarm-check`)
  * :const:`tlp.TulipMaterialDesignIcons.AlarmMultiple` (:mdicon:`alarm-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.AlarmOff` (:mdicon:`alarm-off`)
  * :const:`tlp.TulipMaterialDesignIcons.AlarmPlus` (:mdicon:`alarm-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.AlarmSnooze` (:mdicon:`alarm-snooze`)
  * :const:`tlp.TulipMaterialDesignIcons.Album` (:mdicon:`album`)
  * :const:`tlp.TulipMaterialDesignIcons.Alert` (:mdicon:`alert`)
  * :const:`tlp.TulipMaterialDesignIcons.AlertBox` (:mdicon:`alert-box`)
  * :const:`tlp.TulipMaterialDesignIcons.AlertCircle` (:mdicon:`alert-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.AlertCircleOutline` (:mdicon:`alert-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.AlertDecagram` (:mdicon:`alert-decagram`)
  * :const:`tlp.TulipMaterialDesignIcons.AlertOctagon` (:mdicon:`alert-octagon`)
  * :const:`tlp.TulipMaterialDesignIcons.AlertOctagram` (:mdicon:`alert-octagram`)
  * :const:`tlp.TulipMaterialDesignIcons.AlertOutline` (:mdicon:`alert-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.AllInclusive` (:mdicon:`all-inclusive`)
  * :const:`tlp.TulipMaterialDesignIcons.Alpha` (:mdicon:`alpha`)
  * :const:`tlp.TulipMaterialDesignIcons.Alphabetical` (:mdicon:`alphabetical`)
  * :const:`tlp.TulipMaterialDesignIcons.Altimeter` (:mdicon:`altimeter`)
  * :const:`tlp.TulipMaterialDesignIcons.Amazon` (:mdicon:`amazon`)
  * :const:`tlp.TulipMaterialDesignIcons.AmazonClouddrive` (:mdicon:`amazon-clouddrive`)
  * :const:`tlp.TulipMaterialDesignIcons.Ambulance` (:mdicon:`ambulance`)
  * :const:`tlp.TulipMaterialDesignIcons.Amplifier` (:mdicon:`amplifier`)
  * :const:`tlp.TulipMaterialDesignIcons.Anchor` (:mdicon:`anchor`)
  * :const:`tlp.TulipMaterialDesignIcons.Android` (:mdicon:`android`)
  * :const:`tlp.TulipMaterialDesignIcons.AndroidDebugBridge` (:mdicon:`android-debug-bridge`)
  * :const:`tlp.TulipMaterialDesignIcons.AndroidStudio` (:mdicon:`android-studio`)
  * :const:`tlp.TulipMaterialDesignIcons.Angular` (:mdicon:`angular`)
  * :const:`tlp.TulipMaterialDesignIcons.Angularjs` (:mdicon:`angularjs`)
  * :const:`tlp.TulipMaterialDesignIcons.Animation` (:mdicon:`animation`)
  * :const:`tlp.TulipMaterialDesignIcons.Apple` (:mdicon:`apple`)
  * :const:`tlp.TulipMaterialDesignIcons.AppleFinder` (:mdicon:`apple-finder`)
  * :const:`tlp.TulipMaterialDesignIcons.AppleIos` (:mdicon:`apple-ios`)
  * :const:`tlp.TulipMaterialDesignIcons.AppleKeyboardCaps` (:mdicon:`apple-keyboard-caps`)
  * :const:`tlp.TulipMaterialDesignIcons.AppleKeyboardCommand` (:mdicon:`apple-keyboard-command`)
  * :const:`tlp.TulipMaterialDesignIcons.AppleKeyboardControl` (:mdicon:`apple-keyboard-control`)
  * :const:`tlp.TulipMaterialDesignIcons.AppleKeyboardOption` (:mdicon:`apple-keyboard-option`)
  * :const:`tlp.TulipMaterialDesignIcons.AppleKeyboardShift` (:mdicon:`apple-keyboard-shift`)
  * :const:`tlp.TulipMaterialDesignIcons.AppleMobileme` (:mdicon:`apple-mobileme`)
  * :const:`tlp.TulipMaterialDesignIcons.AppleSafari` (:mdicon:`apple-safari`)
  * :const:`tlp.TulipMaterialDesignIcons.Application` (:mdicon:`application`)
  * :const:`tlp.TulipMaterialDesignIcons.Apps` (:mdicon:`apps`)
  * :const:`tlp.TulipMaterialDesignIcons.Archive` (:mdicon:`archive`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrangeBringForward` (:mdicon:`arrange-bring-forward`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrangeBringToFront` (:mdicon:`arrange-bring-to-front`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrangeSendBackward` (:mdicon:`arrange-send-backward`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrangeSendToBack` (:mdicon:`arrange-send-to-back`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowAll` (:mdicon:`arrow-all`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowBottomLeft` (:mdicon:`arrow-bottom-left`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowBottomRight` (:mdicon:`arrow-bottom-right`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowCompress` (:mdicon:`arrow-compress`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowCompressAll` (:mdicon:`arrow-compress-all`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDown` (:mdicon:`arrow-down`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownBold` (:mdicon:`arrow-down-bold`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownBoldBox` (:mdicon:`arrow-down-bold-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownBoldBoxOutline` (:mdicon:`arrow-down-bold-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownBoldCircle` (:mdicon:`arrow-down-bold-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownBoldCircleOutline` (:mdicon:`arrow-down-bold-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownBoldHexagonOutline` (:mdicon:`arrow-down-bold-hexagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownBox` (:mdicon:`arrow-down-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownDropCircle` (:mdicon:`arrow-down-drop-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownDropCircleOutline` (:mdicon:`arrow-down-drop-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownThick` (:mdicon:`arrow-down-thick`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowExpand` (:mdicon:`arrow-expand`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowExpandAll` (:mdicon:`arrow-expand-all`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeft` (:mdicon:`arrow-left`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBold` (:mdicon:`arrow-left-bold`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBoldBox` (:mdicon:`arrow-left-bold-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBoldBoxOutline` (:mdicon:`arrow-left-bold-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBoldCircle` (:mdicon:`arrow-left-bold-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBoldCircleOutline` (:mdicon:`arrow-left-bold-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBoldHexagonOutline` (:mdicon:`arrow-left-bold-hexagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBox` (:mdicon:`arrow-left-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftDropCircle` (:mdicon:`arrow-left-drop-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftDropCircleOutline` (:mdicon:`arrow-left-drop-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftThick` (:mdicon:`arrow-left-thick`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRight` (:mdicon:`arrow-right`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBold` (:mdicon:`arrow-right-bold`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBoldBox` (:mdicon:`arrow-right-bold-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBoldBoxOutline` (:mdicon:`arrow-right-bold-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBoldCircle` (:mdicon:`arrow-right-bold-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBoldCircleOutline` (:mdicon:`arrow-right-bold-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBoldHexagonOutline` (:mdicon:`arrow-right-bold-hexagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBox` (:mdicon:`arrow-right-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightDropCircle` (:mdicon:`arrow-right-drop-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightDropCircleOutline` (:mdicon:`arrow-right-drop-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightThick` (:mdicon:`arrow-right-thick`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowTopLeft` (:mdicon:`arrow-top-left`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowTopRight` (:mdicon:`arrow-top-right`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUp` (:mdicon:`arrow-up`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBold` (:mdicon:`arrow-up-bold`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBoldBox` (:mdicon:`arrow-up-bold-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBoldBoxOutline` (:mdicon:`arrow-up-bold-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBoldCircle` (:mdicon:`arrow-up-bold-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBoldCircleOutline` (:mdicon:`arrow-up-bold-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBoldHexagonOutline` (:mdicon:`arrow-up-bold-hexagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBox` (:mdicon:`arrow-up-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpDropCircle` (:mdicon:`arrow-up-drop-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpDropCircleOutline` (:mdicon:`arrow-up-drop-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpThick` (:mdicon:`arrow-up-thick`)
  * :const:`tlp.TulipMaterialDesignIcons.Assistant` (:mdicon:`assistant`)
  * :const:`tlp.TulipMaterialDesignIcons.Asterisk` (:mdicon:`asterisk`)
  * :const:`tlp.TulipMaterialDesignIcons.At` (:mdicon:`at`)
  * :const:`tlp.TulipMaterialDesignIcons.Atom` (:mdicon:`atom`)
  * :const:`tlp.TulipMaterialDesignIcons.Attachment` (:mdicon:`attachment`)
  * :const:`tlp.TulipMaterialDesignIcons.Audiobook` (:mdicon:`audiobook`)
  * :const:`tlp.TulipMaterialDesignIcons.AutoFix` (:mdicon:`auto-fix`)
  * :const:`tlp.TulipMaterialDesignIcons.AutoUpload` (:mdicon:`auto-upload`)
  * :const:`tlp.TulipMaterialDesignIcons.Autorenew` (:mdicon:`autorenew`)
  * :const:`tlp.TulipMaterialDesignIcons.AvTimer` (:mdicon:`av-timer`)
  * :const:`tlp.TulipMaterialDesignIcons.Baby` (:mdicon:`baby`)
  * :const:`tlp.TulipMaterialDesignIcons.BabyBuggy` (:mdicon:`baby-buggy`)
  * :const:`tlp.TulipMaterialDesignIcons.Backburger` (:mdicon:`backburger`)
  * :const:`tlp.TulipMaterialDesignIcons.Backspace` (:mdicon:`backspace`)
  * :const:`tlp.TulipMaterialDesignIcons.BackupRestore` (:mdicon:`backup-restore`)
  * :const:`tlp.TulipMaterialDesignIcons.Bandcamp` (:mdicon:`bandcamp`)
  * :const:`tlp.TulipMaterialDesignIcons.Bank` (:mdicon:`bank`)
  * :const:`tlp.TulipMaterialDesignIcons.Barcode` (:mdicon:`barcode`)
  * :const:`tlp.TulipMaterialDesignIcons.BarcodeScan` (:mdicon:`barcode-scan`)
  * :const:`tlp.TulipMaterialDesignIcons.Barley` (:mdicon:`barley`)
  * :const:`tlp.TulipMaterialDesignIcons.Barrel` (:mdicon:`barrel`)
  * :const:`tlp.TulipMaterialDesignIcons.Basecamp` (:mdicon:`basecamp`)
  * :const:`tlp.TulipMaterialDesignIcons.Basket` (:mdicon:`basket`)
  * :const:`tlp.TulipMaterialDesignIcons.BasketFill` (:mdicon:`basket-fill`)
  * :const:`tlp.TulipMaterialDesignIcons.BasketUnfill` (:mdicon:`basket-unfill`)
  * :const:`tlp.TulipMaterialDesignIcons.Battery` (:mdicon:`battery`)
  * :const:`tlp.TulipMaterialDesignIcons.Battery10` (:mdicon:`battery-10`)
  * :const:`tlp.TulipMaterialDesignIcons.Battery20` (:mdicon:`battery-20`)
  * :const:`tlp.TulipMaterialDesignIcons.Battery30` (:mdicon:`battery-30`)
  * :const:`tlp.TulipMaterialDesignIcons.Battery40` (:mdicon:`battery-40`)
  * :const:`tlp.TulipMaterialDesignIcons.Battery50` (:mdicon:`battery-50`)
  * :const:`tlp.TulipMaterialDesignIcons.Battery60` (:mdicon:`battery-60`)
  * :const:`tlp.TulipMaterialDesignIcons.Battery70` (:mdicon:`battery-70`)
  * :const:`tlp.TulipMaterialDesignIcons.Battery80` (:mdicon:`battery-80`)
  * :const:`tlp.TulipMaterialDesignIcons.Battery90` (:mdicon:`battery-90`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryAlert` (:mdicon:`battery-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryCharging` (:mdicon:`battery-charging`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryCharging100` (:mdicon:`battery-charging-100`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryCharging20` (:mdicon:`battery-charging-20`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryCharging30` (:mdicon:`battery-charging-30`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryCharging40` (:mdicon:`battery-charging-40`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryCharging60` (:mdicon:`battery-charging-60`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryCharging80` (:mdicon:`battery-charging-80`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryCharging90` (:mdicon:`battery-charging-90`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryMinus` (:mdicon:`battery-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryNegative` (:mdicon:`battery-negative`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryOutline` (:mdicon:`battery-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryPlus` (:mdicon:`battery-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryPositive` (:mdicon:`battery-positive`)
  * :const:`tlp.TulipMaterialDesignIcons.BatteryUnknown` (:mdicon:`battery-unknown`)
  * :const:`tlp.TulipMaterialDesignIcons.Beach` (:mdicon:`beach`)
  * :const:`tlp.TulipMaterialDesignIcons.Beaker` (:mdicon:`beaker`)
  * :const:`tlp.TulipMaterialDesignIcons.Beats` (:mdicon:`beats`)
  * :const:`tlp.TulipMaterialDesignIcons.Beer` (:mdicon:`beer`)
  * :const:`tlp.TulipMaterialDesignIcons.Behance` (:mdicon:`behance`)
  * :const:`tlp.TulipMaterialDesignIcons.Bell` (:mdicon:`bell`)
  * :const:`tlp.TulipMaterialDesignIcons.BellOff` (:mdicon:`bell-off`)
  * :const:`tlp.TulipMaterialDesignIcons.BellOutline` (:mdicon:`bell-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.BellPlus` (:mdicon:`bell-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.BellRing` (:mdicon:`bell-ring`)
  * :const:`tlp.TulipMaterialDesignIcons.BellRingOutline` (:mdicon:`bell-ring-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.BellSleep` (:mdicon:`bell-sleep`)
  * :const:`tlp.TulipMaterialDesignIcons.Beta` (:mdicon:`beta`)
  * :const:`tlp.TulipMaterialDesignIcons.Bible` (:mdicon:`bible`)
  * :const:`tlp.TulipMaterialDesignIcons.Bike` (:mdicon:`bike`)
  * :const:`tlp.TulipMaterialDesignIcons.Bing` (:mdicon:`bing`)
  * :const:`tlp.TulipMaterialDesignIcons.Binoculars` (:mdicon:`binoculars`)
  * :const:`tlp.TulipMaterialDesignIcons.Bio` (:mdicon:`bio`)
  * :const:`tlp.TulipMaterialDesignIcons.Biohazard` (:mdicon:`biohazard`)
  * :const:`tlp.TulipMaterialDesignIcons.Bitbucket` (:mdicon:`bitbucket`)
  * :const:`tlp.TulipMaterialDesignIcons.BlackMesa` (:mdicon:`black-mesa`)
  * :const:`tlp.TulipMaterialDesignIcons.Blackberry` (:mdicon:`blackberry`)
  * :const:`tlp.TulipMaterialDesignIcons.Blender` (:mdicon:`blender`)
  * :const:`tlp.TulipMaterialDesignIcons.Blinds` (:mdicon:`blinds`)
  * :const:`tlp.TulipMaterialDesignIcons.BlockHelper` (:mdicon:`block-helper`)
  * :const:`tlp.TulipMaterialDesignIcons.Blogger` (:mdicon:`blogger`)
  * :const:`tlp.TulipMaterialDesignIcons.Bluetooth` (:mdicon:`bluetooth`)
  * :const:`tlp.TulipMaterialDesignIcons.BluetoothAudio` (:mdicon:`bluetooth-audio`)
  * :const:`tlp.TulipMaterialDesignIcons.BluetoothConnect` (:mdicon:`bluetooth-connect`)
  * :const:`tlp.TulipMaterialDesignIcons.BluetoothOff` (:mdicon:`bluetooth-off`)
  * :const:`tlp.TulipMaterialDesignIcons.BluetoothSettings` (:mdicon:`bluetooth-settings`)
  * :const:`tlp.TulipMaterialDesignIcons.BluetoothTransfer` (:mdicon:`bluetooth-transfer`)
  * :const:`tlp.TulipMaterialDesignIcons.Blur` (:mdicon:`blur`)
  * :const:`tlp.TulipMaterialDesignIcons.BlurLinear` (:mdicon:`blur-linear`)
  * :const:`tlp.TulipMaterialDesignIcons.BlurOff` (:mdicon:`blur-off`)
  * :const:`tlp.TulipMaterialDesignIcons.BlurRadial` (:mdicon:`blur-radial`)
  * :const:`tlp.TulipMaterialDesignIcons.Bomb` (:mdicon:`bomb`)
  * :const:`tlp.TulipMaterialDesignIcons.BombOff` (:mdicon:`bomb-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Bone` (:mdicon:`bone`)
  * :const:`tlp.TulipMaterialDesignIcons.Book` (:mdicon:`book`)
  * :const:`tlp.TulipMaterialDesignIcons.BookMinus` (:mdicon:`book-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.BookMultiple` (:mdicon:`book-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.BookMultipleVariant` (:mdicon:`book-multiple-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.BookOpen` (:mdicon:`book-open`)
  * :const:`tlp.TulipMaterialDesignIcons.BookOpenPageVariant` (:mdicon:`book-open-page-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.BookOpenVariant` (:mdicon:`book-open-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.BookPlus` (:mdicon:`book-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.BookVariant` (:mdicon:`book-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Bookmark` (:mdicon:`bookmark`)
  * :const:`tlp.TulipMaterialDesignIcons.BookmarkCheck` (:mdicon:`bookmark-check`)
  * :const:`tlp.TulipMaterialDesignIcons.BookmarkMusic` (:mdicon:`bookmark-music`)
  * :const:`tlp.TulipMaterialDesignIcons.BookmarkOutline` (:mdicon:`bookmark-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.BookmarkPlus` (:mdicon:`bookmark-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.BookmarkPlusOutline` (:mdicon:`bookmark-plus-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.BookmarkRemove` (:mdicon:`bookmark-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.Boombox` (:mdicon:`boombox`)
  * :const:`tlp.TulipMaterialDesignIcons.Bootstrap` (:mdicon:`bootstrap`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderAll` (:mdicon:`border-all`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderBottom` (:mdicon:`border-bottom`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderColor` (:mdicon:`border-color`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderHorizontal` (:mdicon:`border-horizontal`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderInside` (:mdicon:`border-inside`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderLeft` (:mdicon:`border-left`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderNone` (:mdicon:`border-none`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderOutside` (:mdicon:`border-outside`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderRight` (:mdicon:`border-right`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderStyle` (:mdicon:`border-style`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderTop` (:mdicon:`border-top`)
  * :const:`tlp.TulipMaterialDesignIcons.BorderVertical` (:mdicon:`border-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.BowTie` (:mdicon:`bow-tie`)
  * :const:`tlp.TulipMaterialDesignIcons.Bowl` (:mdicon:`bowl`)
  * :const:`tlp.TulipMaterialDesignIcons.Bowling` (:mdicon:`bowling`)
  * :const:`tlp.TulipMaterialDesignIcons.Box` (:mdicon:`box`)
  * :const:`tlp.TulipMaterialDesignIcons.BoxCutter` (:mdicon:`box-cutter`)
  * :const:`tlp.TulipMaterialDesignIcons.BoxShadow` (:mdicon:`box-shadow`)
  * :const:`tlp.TulipMaterialDesignIcons.Bridge` (:mdicon:`bridge`)
  * :const:`tlp.TulipMaterialDesignIcons.Briefcase` (:mdicon:`briefcase`)
  * :const:`tlp.TulipMaterialDesignIcons.BriefcaseCheck` (:mdicon:`briefcase-check`)
  * :const:`tlp.TulipMaterialDesignIcons.BriefcaseDownload` (:mdicon:`briefcase-download`)
  * :const:`tlp.TulipMaterialDesignIcons.BriefcaseUpload` (:mdicon:`briefcase-upload`)
  * :const:`tlp.TulipMaterialDesignIcons.Brightness1` (:mdicon:`brightness-1`)
  * :const:`tlp.TulipMaterialDesignIcons.Brightness2` (:mdicon:`brightness-2`)
  * :const:`tlp.TulipMaterialDesignIcons.Brightness3` (:mdicon:`brightness-3`)
  * :const:`tlp.TulipMaterialDesignIcons.Brightness4` (:mdicon:`brightness-4`)
  * :const:`tlp.TulipMaterialDesignIcons.Brightness5` (:mdicon:`brightness-5`)
  * :const:`tlp.TulipMaterialDesignIcons.Brightness6` (:mdicon:`brightness-6`)
  * :const:`tlp.TulipMaterialDesignIcons.Brightness7` (:mdicon:`brightness-7`)
  * :const:`tlp.TulipMaterialDesignIcons.BrightnessAuto` (:mdicon:`brightness-auto`)
  * :const:`tlp.TulipMaterialDesignIcons.Broom` (:mdicon:`broom`)
  * :const:`tlp.TulipMaterialDesignIcons.Brush` (:mdicon:`brush`)
  * :const:`tlp.TulipMaterialDesignIcons.Buffer` (:mdicon:`buffer`)
  * :const:`tlp.TulipMaterialDesignIcons.Bug` (:mdicon:`bug`)
  * :const:`tlp.TulipMaterialDesignIcons.BulletinBoard` (:mdicon:`bulletin-board`)
  * :const:`tlp.TulipMaterialDesignIcons.Bullhorn` (:mdicon:`bullhorn`)
  * :const:`tlp.TulipMaterialDesignIcons.Bullseye` (:mdicon:`bullseye`)
  * :const:`tlp.TulipMaterialDesignIcons.BurstMode` (:mdicon:`burst-mode`)
  * :const:`tlp.TulipMaterialDesignIcons.Bus` (:mdicon:`bus`)
  * :const:`tlp.TulipMaterialDesignIcons.Cached` (:mdicon:`cached`)
  * :const:`tlp.TulipMaterialDesignIcons.Cake` (:mdicon:`cake`)
  * :const:`tlp.TulipMaterialDesignIcons.CakeLayered` (:mdicon:`cake-layered`)
  * :const:`tlp.TulipMaterialDesignIcons.CakeVariant` (:mdicon:`cake-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Calculator` (:mdicon:`calculator`)
  * :const:`tlp.TulipMaterialDesignIcons.Calendar` (:mdicon:`calendar`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarBlank` (:mdicon:`calendar-blank`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarCheck` (:mdicon:`calendar-check`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarClock` (:mdicon:`calendar-clock`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarMultiple` (:mdicon:`calendar-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarMultipleCheck` (:mdicon:`calendar-multiple-check`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarPlus` (:mdicon:`calendar-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarQuestion` (:mdicon:`calendar-question`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarRange` (:mdicon:`calendar-range`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarRemove` (:mdicon:`calendar-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarText` (:mdicon:`calendar-text`)
  * :const:`tlp.TulipMaterialDesignIcons.CalendarToday` (:mdicon:`calendar-today`)
  * :const:`tlp.TulipMaterialDesignIcons.CallMade` (:mdicon:`call-made`)
  * :const:`tlp.TulipMaterialDesignIcons.CallMerge` (:mdicon:`call-merge`)
  * :const:`tlp.TulipMaterialDesignIcons.CallMissed` (:mdicon:`call-missed`)
  * :const:`tlp.TulipMaterialDesignIcons.CallReceived` (:mdicon:`call-received`)
  * :const:`tlp.TulipMaterialDesignIcons.CallSplit` (:mdicon:`call-split`)
  * :const:`tlp.TulipMaterialDesignIcons.Camcorder` (:mdicon:`camcorder`)
  * :const:`tlp.TulipMaterialDesignIcons.CamcorderBox` (:mdicon:`camcorder-box`)
  * :const:`tlp.TulipMaterialDesignIcons.CamcorderBoxOff` (:mdicon:`camcorder-box-off`)
  * :const:`tlp.TulipMaterialDesignIcons.CamcorderOff` (:mdicon:`camcorder-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Camera` (:mdicon:`camera`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraBurst` (:mdicon:`camera-burst`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraEnhance` (:mdicon:`camera-enhance`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraFront` (:mdicon:`camera-front`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraFrontVariant` (:mdicon:`camera-front-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraIris` (:mdicon:`camera-iris`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraOff` (:mdicon:`camera-off`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraPartyMode` (:mdicon:`camera-party-mode`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraRear` (:mdicon:`camera-rear`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraRearVariant` (:mdicon:`camera-rear-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraSwitch` (:mdicon:`camera-switch`)
  * :const:`tlp.TulipMaterialDesignIcons.CameraTimer` (:mdicon:`camera-timer`)
  * :const:`tlp.TulipMaterialDesignIcons.Cancel` (:mdicon:`cancel`)
  * :const:`tlp.TulipMaterialDesignIcons.Candle` (:mdicon:`candle`)
  * :const:`tlp.TulipMaterialDesignIcons.Candycane` (:mdicon:`candycane`)
  * :const:`tlp.TulipMaterialDesignIcons.Car` (:mdicon:`car`)
  * :const:`tlp.TulipMaterialDesignIcons.CarBattery` (:mdicon:`car-battery`)
  * :const:`tlp.TulipMaterialDesignIcons.CarConnected` (:mdicon:`car-connected`)
  * :const:`tlp.TulipMaterialDesignIcons.CarWash` (:mdicon:`car-wash`)
  * :const:`tlp.TulipMaterialDesignIcons.Cards` (:mdicon:`cards`)
  * :const:`tlp.TulipMaterialDesignIcons.CardsOutline` (:mdicon:`cards-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CardsPlayingOutline` (:mdicon:`cards-playing-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CardsVariant` (:mdicon:`cards-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Carrot` (:mdicon:`carrot`)
  * :const:`tlp.TulipMaterialDesignIcons.Cart` (:mdicon:`cart`)
  * :const:`tlp.TulipMaterialDesignIcons.CartOff` (:mdicon:`cart-off`)
  * :const:`tlp.TulipMaterialDesignIcons.CartOutline` (:mdicon:`cart-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CartPlus` (:mdicon:`cart-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.CaseSensitiveAlt` (:mdicon:`case-sensitive-alt`)
  * :const:`tlp.TulipMaterialDesignIcons.Cash` (:mdicon:`cash`)
  * :const:`tlp.TulipMaterialDesignIcons.Cash100` (:mdicon:`cash-100`)
  * :const:`tlp.TulipMaterialDesignIcons.CashMultiple` (:mdicon:`cash-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.CashUsd` (:mdicon:`cash-usd`)
  * :const:`tlp.TulipMaterialDesignIcons.Cast` (:mdicon:`cast`)
  * :const:`tlp.TulipMaterialDesignIcons.CastConnected` (:mdicon:`cast-connected`)
  * :const:`tlp.TulipMaterialDesignIcons.CastOff` (:mdicon:`cast-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Castle` (:mdicon:`castle`)
  * :const:`tlp.TulipMaterialDesignIcons.Cat` (:mdicon:`cat`)
  * :const:`tlp.TulipMaterialDesignIcons.CeilingLight` (:mdicon:`ceiling-light`)
  * :const:`tlp.TulipMaterialDesignIcons.Cellphone` (:mdicon:`cellphone`)
  * :const:`tlp.TulipMaterialDesignIcons.CellphoneAndroid` (:mdicon:`cellphone-android`)
  * :const:`tlp.TulipMaterialDesignIcons.CellphoneBasic` (:mdicon:`cellphone-basic`)
  * :const:`tlp.TulipMaterialDesignIcons.CellphoneDock` (:mdicon:`cellphone-dock`)
  * :const:`tlp.TulipMaterialDesignIcons.CellphoneIphone` (:mdicon:`cellphone-iphone`)
  * :const:`tlp.TulipMaterialDesignIcons.CellphoneLink` (:mdicon:`cellphone-link`)
  * :const:`tlp.TulipMaterialDesignIcons.CellphoneLinkOff` (:mdicon:`cellphone-link-off`)
  * :const:`tlp.TulipMaterialDesignIcons.CellphoneSettings` (:mdicon:`cellphone-settings`)
  * :const:`tlp.TulipMaterialDesignIcons.Certificate` (:mdicon:`certificate`)
  * :const:`tlp.TulipMaterialDesignIcons.ChairSchool` (:mdicon:`chair-school`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartArc` (:mdicon:`chart-arc`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartAreaspline` (:mdicon:`chart-areaspline`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartBar` (:mdicon:`chart-bar`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartBarStacked` (:mdicon:`chart-bar-stacked`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartBubble` (:mdicon:`chart-bubble`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartGantt` (:mdicon:`chart-gantt`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartHistogram` (:mdicon:`chart-histogram`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartLine` (:mdicon:`chart-line`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartLineStacked` (:mdicon:`chart-line-stacked`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartPie` (:mdicon:`chart-pie`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartScatterplotHexbin` (:mdicon:`chart-scatterplot-hexbin`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartTimeline` (:mdicon:`chart-timeline`)
  * :const:`tlp.TulipMaterialDesignIcons.Check` (:mdicon:`check`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckAll` (:mdicon:`check-all`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckCircle` (:mdicon:`check-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckCircleOutline` (:mdicon:`check-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxBlank` (:mdicon:`checkbox-blank`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxBlankCircle` (:mdicon:`checkbox-blank-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxBlankCircleOutline` (:mdicon:`checkbox-blank-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxBlankOutline` (:mdicon:`checkbox-blank-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMarked` (:mdicon:`checkbox-marked`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMarkedCircle` (:mdicon:`checkbox-marked-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMarkedCircleOutline` (:mdicon:`checkbox-marked-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMarkedOutline` (:mdicon:`checkbox-marked-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMultipleBlank` (:mdicon:`checkbox-multiple-blank`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMultipleBlankCircle` (:mdicon:`checkbox-multiple-blank-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMultipleBlankCircleOutline` (:mdicon:`checkbox-multiple-blank-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMultipleBlankOutline` (:mdicon:`checkbox-multiple-blank-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMultipleMarked` (:mdicon:`checkbox-multiple-marked`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMultipleMarkedCircle` (:mdicon:`checkbox-multiple-marked-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMultipleMarkedCircleOutline` (:mdicon:`checkbox-multiple-marked-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CheckboxMultipleMarkedOutline` (:mdicon:`checkbox-multiple-marked-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Checkerboard` (:mdicon:`checkerboard`)
  * :const:`tlp.TulipMaterialDesignIcons.ChemicalWeapon` (:mdicon:`chemical-weapon`)
  * :const:`tlp.TulipMaterialDesignIcons.ChevronDoubleDown` (:mdicon:`chevron-double-down`)
  * :const:`tlp.TulipMaterialDesignIcons.ChevronDoubleLeft` (:mdicon:`chevron-double-left`)
  * :const:`tlp.TulipMaterialDesignIcons.ChevronDoubleRight` (:mdicon:`chevron-double-right`)
  * :const:`tlp.TulipMaterialDesignIcons.ChevronDoubleUp` (:mdicon:`chevron-double-up`)
  * :const:`tlp.TulipMaterialDesignIcons.ChevronDown` (:mdicon:`chevron-down`)
  * :const:`tlp.TulipMaterialDesignIcons.ChevronLeft` (:mdicon:`chevron-left`)
  * :const:`tlp.TulipMaterialDesignIcons.ChevronRight` (:mdicon:`chevron-right`)
  * :const:`tlp.TulipMaterialDesignIcons.ChevronUp` (:mdicon:`chevron-up`)
  * :const:`tlp.TulipMaterialDesignIcons.Chip` (:mdicon:`chip`)
  * :const:`tlp.TulipMaterialDesignIcons.Church` (:mdicon:`church`)
  * :const:`tlp.TulipMaterialDesignIcons.Circle` (:mdicon:`circle`)
  * :const:`tlp.TulipMaterialDesignIcons.CircleOutline` (:mdicon:`circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CiscoWebex` (:mdicon:`cisco-webex`)
  * :const:`tlp.TulipMaterialDesignIcons.City` (:mdicon:`city`)
  * :const:`tlp.TulipMaterialDesignIcons.Clipboard` (:mdicon:`clipboard`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardAccount` (:mdicon:`clipboard-account`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardAlert` (:mdicon:`clipboard-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardArrowDown` (:mdicon:`clipboard-arrow-down`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardArrowLeft` (:mdicon:`clipboard-arrow-left`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardCheck` (:mdicon:`clipboard-check`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardFlow` (:mdicon:`clipboard-flow`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardOutline` (:mdicon:`clipboard-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardPlus` (:mdicon:`clipboard-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardText` (:mdicon:`clipboard-text`)
  * :const:`tlp.TulipMaterialDesignIcons.Clippy` (:mdicon:`clippy`)
  * :const:`tlp.TulipMaterialDesignIcons.Clock` (:mdicon:`clock`)
  * :const:`tlp.TulipMaterialDesignIcons.ClockAlert` (:mdicon:`clock-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.ClockEnd` (:mdicon:`clock-end`)
  * :const:`tlp.TulipMaterialDesignIcons.ClockFast` (:mdicon:`clock-fast`)
  * :const:`tlp.TulipMaterialDesignIcons.ClockIn` (:mdicon:`clock-in`)
  * :const:`tlp.TulipMaterialDesignIcons.ClockOut` (:mdicon:`clock-out`)
  * :const:`tlp.TulipMaterialDesignIcons.ClockStart` (:mdicon:`clock-start`)
  * :const:`tlp.TulipMaterialDesignIcons.Close` (:mdicon:`close`)
  * :const:`tlp.TulipMaterialDesignIcons.CloseBox` (:mdicon:`close-box`)
  * :const:`tlp.TulipMaterialDesignIcons.CloseBoxOutline` (:mdicon:`close-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CloseCircle` (:mdicon:`close-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.CloseCircleOutline` (:mdicon:`close-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CloseNetwork` (:mdicon:`close-network`)
  * :const:`tlp.TulipMaterialDesignIcons.CloseOctagon` (:mdicon:`close-octagon`)
  * :const:`tlp.TulipMaterialDesignIcons.CloseOctagonOutline` (:mdicon:`close-octagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CloseOutline` (:mdicon:`close-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ClosedCaption` (:mdicon:`closed-caption`)
  * :const:`tlp.TulipMaterialDesignIcons.Cloud` (:mdicon:`cloud`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudCheck` (:mdicon:`cloud-check`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudCircle` (:mdicon:`cloud-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudDownload` (:mdicon:`cloud-download`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudOffOutline` (:mdicon:`cloud-off-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudOutline` (:mdicon:`cloud-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudPrint` (:mdicon:`cloud-print`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudPrintOutline` (:mdicon:`cloud-print-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudSync` (:mdicon:`cloud-sync`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudUpload` (:mdicon:`cloud-upload`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeArray` (:mdicon:`code-array`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeBraces` (:mdicon:`code-braces`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeBrackets` (:mdicon:`code-brackets`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeEqual` (:mdicon:`code-equal`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeGreaterThan` (:mdicon:`code-greater-than`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeGreaterThanOrEqual` (:mdicon:`code-greater-than-or-equal`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeLessThan` (:mdicon:`code-less-than`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeLessThanOrEqual` (:mdicon:`code-less-than-or-equal`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeNotEqual` (:mdicon:`code-not-equal`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeNotEqualVariant` (:mdicon:`code-not-equal-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeParentheses` (:mdicon:`code-parentheses`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeString` (:mdicon:`code-string`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeTags` (:mdicon:`code-tags`)
  * :const:`tlp.TulipMaterialDesignIcons.CodeTagsCheck` (:mdicon:`code-tags-check`)
  * :const:`tlp.TulipMaterialDesignIcons.Codepen` (:mdicon:`codepen`)
  * :const:`tlp.TulipMaterialDesignIcons.Coffee` (:mdicon:`coffee`)
  * :const:`tlp.TulipMaterialDesignIcons.CoffeeOutline` (:mdicon:`coffee-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CoffeeToGo` (:mdicon:`coffee-to-go`)
  * :const:`tlp.TulipMaterialDesignIcons.Coin` (:mdicon:`coin`)
  * :const:`tlp.TulipMaterialDesignIcons.Coins` (:mdicon:`coins`)
  * :const:`tlp.TulipMaterialDesignIcons.Collage` (:mdicon:`collage`)
  * :const:`tlp.TulipMaterialDesignIcons.ColorHelper` (:mdicon:`color-helper`)
  * :const:`tlp.TulipMaterialDesignIcons.Comment` (:mdicon:`comment`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentAccount` (:mdicon:`comment-account`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentAccountOutline` (:mdicon:`comment-account-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentAlert` (:mdicon:`comment-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentAlertOutline` (:mdicon:`comment-alert-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentCheck` (:mdicon:`comment-check`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentCheckOutline` (:mdicon:`comment-check-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentMultipleOutline` (:mdicon:`comment-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentOutline` (:mdicon:`comment-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentPlusOutline` (:mdicon:`comment-plus-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentProcessing` (:mdicon:`comment-processing`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentProcessingOutline` (:mdicon:`comment-processing-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentQuestionOutline` (:mdicon:`comment-question-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentRemoveOutline` (:mdicon:`comment-remove-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentText` (:mdicon:`comment-text`)
  * :const:`tlp.TulipMaterialDesignIcons.CommentTextOutline` (:mdicon:`comment-text-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Compare` (:mdicon:`compare`)
  * :const:`tlp.TulipMaterialDesignIcons.Compass` (:mdicon:`compass`)
  * :const:`tlp.TulipMaterialDesignIcons.CompassOutline` (:mdicon:`compass-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Console` (:mdicon:`console`)
  * :const:`tlp.TulipMaterialDesignIcons.ContactMail` (:mdicon:`contact-mail`)
  * :const:`tlp.TulipMaterialDesignIcons.Contacts` (:mdicon:`contacts`)
  * :const:`tlp.TulipMaterialDesignIcons.ContentCopy` (:mdicon:`content-copy`)
  * :const:`tlp.TulipMaterialDesignIcons.ContentCut` (:mdicon:`content-cut`)
  * :const:`tlp.TulipMaterialDesignIcons.ContentDuplicate` (:mdicon:`content-duplicate`)
  * :const:`tlp.TulipMaterialDesignIcons.ContentPaste` (:mdicon:`content-paste`)
  * :const:`tlp.TulipMaterialDesignIcons.ContentSave` (:mdicon:`content-save`)
  * :const:`tlp.TulipMaterialDesignIcons.ContentSaveAll` (:mdicon:`content-save-all`)
  * :const:`tlp.TulipMaterialDesignIcons.ContentSaveSettings` (:mdicon:`content-save-settings`)
  * :const:`tlp.TulipMaterialDesignIcons.Contrast` (:mdicon:`contrast`)
  * :const:`tlp.TulipMaterialDesignIcons.ContrastBox` (:mdicon:`contrast-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ContrastCircle` (:mdicon:`contrast-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.Cookie` (:mdicon:`cookie`)
  * :const:`tlp.TulipMaterialDesignIcons.Copyright` (:mdicon:`copyright`)
  * :const:`tlp.TulipMaterialDesignIcons.Counter` (:mdicon:`counter`)
  * :const:`tlp.TulipMaterialDesignIcons.Cow` (:mdicon:`cow`)
  * :const:`tlp.TulipMaterialDesignIcons.Creation` (:mdicon:`creation`)
  * :const:`tlp.TulipMaterialDesignIcons.CreditCard` (:mdicon:`credit-card`)
  * :const:`tlp.TulipMaterialDesignIcons.CreditCardMultiple` (:mdicon:`credit-card-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.CreditCardOff` (:mdicon:`credit-card-off`)
  * :const:`tlp.TulipMaterialDesignIcons.CreditCardPlus` (:mdicon:`credit-card-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.CreditCardScan` (:mdicon:`credit-card-scan`)
  * :const:`tlp.TulipMaterialDesignIcons.Crop` (:mdicon:`crop`)
  * :const:`tlp.TulipMaterialDesignIcons.CropFree` (:mdicon:`crop-free`)
  * :const:`tlp.TulipMaterialDesignIcons.CropLandscape` (:mdicon:`crop-landscape`)
  * :const:`tlp.TulipMaterialDesignIcons.CropPortrait` (:mdicon:`crop-portrait`)
  * :const:`tlp.TulipMaterialDesignIcons.CropRotate` (:mdicon:`crop-rotate`)
  * :const:`tlp.TulipMaterialDesignIcons.CropSquare` (:mdicon:`crop-square`)
  * :const:`tlp.TulipMaterialDesignIcons.Crosshairs` (:mdicon:`crosshairs`)
  * :const:`tlp.TulipMaterialDesignIcons.CrosshairsGps` (:mdicon:`crosshairs-gps`)
  * :const:`tlp.TulipMaterialDesignIcons.Crown` (:mdicon:`crown`)
  * :const:`tlp.TulipMaterialDesignIcons.Cube` (:mdicon:`cube`)
  * :const:`tlp.TulipMaterialDesignIcons.CubeOutline` (:mdicon:`cube-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CubeSend` (:mdicon:`cube-send`)
  * :const:`tlp.TulipMaterialDesignIcons.CubeUnfolded` (:mdicon:`cube-unfolded`)
  * :const:`tlp.TulipMaterialDesignIcons.Cup` (:mdicon:`cup`)
  * :const:`tlp.TulipMaterialDesignIcons.CupOff` (:mdicon:`cup-off`)
  * :const:`tlp.TulipMaterialDesignIcons.CupWater` (:mdicon:`cup-water`)
  * :const:`tlp.TulipMaterialDesignIcons.CurrencyBtc` (:mdicon:`currency-btc`)
  * :const:`tlp.TulipMaterialDesignIcons.CurrencyEur` (:mdicon:`currency-eur`)
  * :const:`tlp.TulipMaterialDesignIcons.CurrencyGbp` (:mdicon:`currency-gbp`)
  * :const:`tlp.TulipMaterialDesignIcons.CurrencyInr` (:mdicon:`currency-inr`)
  * :const:`tlp.TulipMaterialDesignIcons.CurrencyNgn` (:mdicon:`currency-ngn`)
  * :const:`tlp.TulipMaterialDesignIcons.CurrencyRub` (:mdicon:`currency-rub`)
  * :const:`tlp.TulipMaterialDesignIcons.CurrencyTry` (:mdicon:`currency-try`)
  * :const:`tlp.TulipMaterialDesignIcons.CurrencyUsd` (:mdicon:`currency-usd`)
  * :const:`tlp.TulipMaterialDesignIcons.CurrencyUsdOff` (:mdicon:`currency-usd-off`)
  * :const:`tlp.TulipMaterialDesignIcons.CursorDefault` (:mdicon:`cursor-default`)
  * :const:`tlp.TulipMaterialDesignIcons.CursorDefaultOutline` (:mdicon:`cursor-default-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CursorMove` (:mdicon:`cursor-move`)
  * :const:`tlp.TulipMaterialDesignIcons.CursorPointer` (:mdicon:`cursor-pointer`)
  * :const:`tlp.TulipMaterialDesignIcons.CursorText` (:mdicon:`cursor-text`)
  * :const:`tlp.TulipMaterialDesignIcons.Database` (:mdicon:`database`)
  * :const:`tlp.TulipMaterialDesignIcons.DatabaseMinus` (:mdicon:`database-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.DatabasePlus` (:mdicon:`database-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.DebugStepInto` (:mdicon:`debug-step-into`)
  * :const:`tlp.TulipMaterialDesignIcons.DebugStepOut` (:mdicon:`debug-step-out`)
  * :const:`tlp.TulipMaterialDesignIcons.DebugStepOver` (:mdicon:`debug-step-over`)
  * :const:`tlp.TulipMaterialDesignIcons.Decagram` (:mdicon:`decagram`)
  * :const:`tlp.TulipMaterialDesignIcons.DecagramOutline` (:mdicon:`decagram-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.DecimalDecrease` (:mdicon:`decimal-decrease`)
  * :const:`tlp.TulipMaterialDesignIcons.DecimalIncrease` (:mdicon:`decimal-increase`)
  * :const:`tlp.TulipMaterialDesignIcons.Delete` (:mdicon:`delete`)
  * :const:`tlp.TulipMaterialDesignIcons.DeleteCircle` (:mdicon:`delete-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.DeleteEmpty` (:mdicon:`delete-empty`)
  * :const:`tlp.TulipMaterialDesignIcons.DeleteForever` (:mdicon:`delete-forever`)
  * :const:`tlp.TulipMaterialDesignIcons.DeleteSweep` (:mdicon:`delete-sweep`)
  * :const:`tlp.TulipMaterialDesignIcons.DeleteVariant` (:mdicon:`delete-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Delta` (:mdicon:`delta`)
  * :const:`tlp.TulipMaterialDesignIcons.Deskphone` (:mdicon:`deskphone`)
  * :const:`tlp.TulipMaterialDesignIcons.DesktopMac` (:mdicon:`desktop-mac`)
  * :const:`tlp.TulipMaterialDesignIcons.DesktopTower` (:mdicon:`desktop-tower`)
  * :const:`tlp.TulipMaterialDesignIcons.Details` (:mdicon:`details`)
  * :const:`tlp.TulipMaterialDesignIcons.DeveloperBoard` (:mdicon:`developer-board`)
  * :const:`tlp.TulipMaterialDesignIcons.Deviantart` (:mdicon:`deviantart`)
  * :const:`tlp.TulipMaterialDesignIcons.Dialpad` (:mdicon:`dialpad`)
  * :const:`tlp.TulipMaterialDesignIcons.Diamond` (:mdicon:`diamond`)
  * :const:`tlp.TulipMaterialDesignIcons.Dice1` (:mdicon:`dice-1`)
  * :const:`tlp.TulipMaterialDesignIcons.Dice2` (:mdicon:`dice-2`)
  * :const:`tlp.TulipMaterialDesignIcons.Dice3` (:mdicon:`dice-3`)
  * :const:`tlp.TulipMaterialDesignIcons.Dice4` (:mdicon:`dice-4`)
  * :const:`tlp.TulipMaterialDesignIcons.Dice5` (:mdicon:`dice-5`)
  * :const:`tlp.TulipMaterialDesignIcons.Dice6` (:mdicon:`dice-6`)
  * :const:`tlp.TulipMaterialDesignIcons.DiceD10` (:mdicon:`dice-d10`)
  * :const:`tlp.TulipMaterialDesignIcons.DiceD20` (:mdicon:`dice-d20`)
  * :const:`tlp.TulipMaterialDesignIcons.DiceD4` (:mdicon:`dice-d4`)
  * :const:`tlp.TulipMaterialDesignIcons.DiceD6` (:mdicon:`dice-d6`)
  * :const:`tlp.TulipMaterialDesignIcons.DiceD8` (:mdicon:`dice-d8`)
  * :const:`tlp.TulipMaterialDesignIcons.DiceMultiple` (:mdicon:`dice-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.Dictionary` (:mdicon:`dictionary`)
  * :const:`tlp.TulipMaterialDesignIcons.Directions` (:mdicon:`directions`)
  * :const:`tlp.TulipMaterialDesignIcons.DirectionsFork` (:mdicon:`directions-fork`)
  * :const:`tlp.TulipMaterialDesignIcons.Discord` (:mdicon:`discord`)
  * :const:`tlp.TulipMaterialDesignIcons.Disk` (:mdicon:`disk`)
  * :const:`tlp.TulipMaterialDesignIcons.DiskAlert` (:mdicon:`disk-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.Disqus` (:mdicon:`disqus`)
  * :const:`tlp.TulipMaterialDesignIcons.DisqusOutline` (:mdicon:`disqus-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Division` (:mdicon:`division`)
  * :const:`tlp.TulipMaterialDesignIcons.DivisionBox` (:mdicon:`division-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Dna` (:mdicon:`dna`)
  * :const:`tlp.TulipMaterialDesignIcons.Dns` (:mdicon:`dns`)
  * :const:`tlp.TulipMaterialDesignIcons.DoNotDisturb` (:mdicon:`do-not-disturb`)
  * :const:`tlp.TulipMaterialDesignIcons.DoNotDisturbOff` (:mdicon:`do-not-disturb-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Dolby` (:mdicon:`dolby`)
  * :const:`tlp.TulipMaterialDesignIcons.Domain` (:mdicon:`domain`)
  * :const:`tlp.TulipMaterialDesignIcons.DotsHorizontal` (:mdicon:`dots-horizontal`)
  * :const:`tlp.TulipMaterialDesignIcons.DotsVertical` (:mdicon:`dots-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.Douban` (:mdicon:`douban`)
  * :const:`tlp.TulipMaterialDesignIcons.Download` (:mdicon:`download`)
  * :const:`tlp.TulipMaterialDesignIcons.DownloadNetwork` (:mdicon:`download-network`)
  * :const:`tlp.TulipMaterialDesignIcons.Drag` (:mdicon:`drag`)
  * :const:`tlp.TulipMaterialDesignIcons.DragHorizontal` (:mdicon:`drag-horizontal`)
  * :const:`tlp.TulipMaterialDesignIcons.DragVertical` (:mdicon:`drag-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.Drawing` (:mdicon:`drawing`)
  * :const:`tlp.TulipMaterialDesignIcons.DrawingBox` (:mdicon:`drawing-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Dribbble` (:mdicon:`dribbble`)
  * :const:`tlp.TulipMaterialDesignIcons.DribbbleBox` (:mdicon:`dribbble-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Drone` (:mdicon:`drone`)
  * :const:`tlp.TulipMaterialDesignIcons.Dropbox` (:mdicon:`dropbox`)
  * :const:`tlp.TulipMaterialDesignIcons.Drupal` (:mdicon:`drupal`)
  * :const:`tlp.TulipMaterialDesignIcons.Duck` (:mdicon:`duck`)
  * :const:`tlp.TulipMaterialDesignIcons.Dumbbell` (:mdicon:`dumbbell`)
  * :const:`tlp.TulipMaterialDesignIcons.Earth` (:mdicon:`earth`)
  * :const:`tlp.TulipMaterialDesignIcons.EarthBox` (:mdicon:`earth-box`)
  * :const:`tlp.TulipMaterialDesignIcons.EarthBoxOff` (:mdicon:`earth-box-off`)
  * :const:`tlp.TulipMaterialDesignIcons.EarthOff` (:mdicon:`earth-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Edge` (:mdicon:`edge`)
  * :const:`tlp.TulipMaterialDesignIcons.Eject` (:mdicon:`eject`)
  * :const:`tlp.TulipMaterialDesignIcons.ElevationDecline` (:mdicon:`elevation-decline`)
  * :const:`tlp.TulipMaterialDesignIcons.ElevationRise` (:mdicon:`elevation-rise`)
  * :const:`tlp.TulipMaterialDesignIcons.Elevator` (:mdicon:`elevator`)
  * :const:`tlp.TulipMaterialDesignIcons.Email` (:mdicon:`email`)
  * :const:`tlp.TulipMaterialDesignIcons.EmailAlert` (:mdicon:`email-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.EmailOpen` (:mdicon:`email-open`)
  * :const:`tlp.TulipMaterialDesignIcons.EmailOpenOutline` (:mdicon:`email-open-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.EmailOutline` (:mdicon:`email-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.EmailSecure` (:mdicon:`email-secure`)
  * :const:`tlp.TulipMaterialDesignIcons.EmailVariant` (:mdicon:`email-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Emby` (:mdicon:`emby`)
  * :const:`tlp.TulipMaterialDesignIcons.Emoticon` (:mdicon:`emoticon`)
  * :const:`tlp.TulipMaterialDesignIcons.EmoticonCool` (:mdicon:`emoticon-cool`)
  * :const:`tlp.TulipMaterialDesignIcons.EmoticonDead` (:mdicon:`emoticon-dead`)
  * :const:`tlp.TulipMaterialDesignIcons.EmoticonDevil` (:mdicon:`emoticon-devil`)
  * :const:`tlp.TulipMaterialDesignIcons.EmoticonExcited` (:mdicon:`emoticon-excited`)
  * :const:`tlp.TulipMaterialDesignIcons.EmoticonHappy` (:mdicon:`emoticon-happy`)
  * :const:`tlp.TulipMaterialDesignIcons.EmoticonNeutral` (:mdicon:`emoticon-neutral`)
  * :const:`tlp.TulipMaterialDesignIcons.EmoticonPoop` (:mdicon:`emoticon-poop`)
  * :const:`tlp.TulipMaterialDesignIcons.EmoticonSad` (:mdicon:`emoticon-sad`)
  * :const:`tlp.TulipMaterialDesignIcons.EmoticonTongue` (:mdicon:`emoticon-tongue`)
  * :const:`tlp.TulipMaterialDesignIcons.Engine` (:mdicon:`engine`)
  * :const:`tlp.TulipMaterialDesignIcons.EngineOutline` (:mdicon:`engine-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Equal` (:mdicon:`equal`)
  * :const:`tlp.TulipMaterialDesignIcons.EqualBox` (:mdicon:`equal-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Eraser` (:mdicon:`eraser`)
  * :const:`tlp.TulipMaterialDesignIcons.EraserVariant` (:mdicon:`eraser-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Escalator` (:mdicon:`escalator`)
  * :const:`tlp.TulipMaterialDesignIcons.Ethernet` (:mdicon:`ethernet`)
  * :const:`tlp.TulipMaterialDesignIcons.EthernetCable` (:mdicon:`ethernet-cable`)
  * :const:`tlp.TulipMaterialDesignIcons.EthernetCableOff` (:mdicon:`ethernet-cable-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Etsy` (:mdicon:`etsy`)
  * :const:`tlp.TulipMaterialDesignIcons.EvStation` (:mdicon:`ev-station`)
  * :const:`tlp.TulipMaterialDesignIcons.Evernote` (:mdicon:`evernote`)
  * :const:`tlp.TulipMaterialDesignIcons.Exclamation` (:mdicon:`exclamation`)
  * :const:`tlp.TulipMaterialDesignIcons.ExitToApp` (:mdicon:`exit-to-app`)
  * :const:`tlp.TulipMaterialDesignIcons.Export` (:mdicon:`export`)
  * :const:`tlp.TulipMaterialDesignIcons.Eye` (:mdicon:`eye`)
  * :const:`tlp.TulipMaterialDesignIcons.EyeOff` (:mdicon:`eye-off`)
  * :const:`tlp.TulipMaterialDesignIcons.EyeOffOutline` (:mdicon:`eye-off-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.EyeOutline` (:mdicon:`eye-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Eyedropper` (:mdicon:`eyedropper`)
  * :const:`tlp.TulipMaterialDesignIcons.EyedropperVariant` (:mdicon:`eyedropper-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Face` (:mdicon:`face`)
  * :const:`tlp.TulipMaterialDesignIcons.FaceProfile` (:mdicon:`face-profile`)
  * :const:`tlp.TulipMaterialDesignIcons.Facebook` (:mdicon:`facebook`)
  * :const:`tlp.TulipMaterialDesignIcons.FacebookBox` (:mdicon:`facebook-box`)
  * :const:`tlp.TulipMaterialDesignIcons.FacebookMessenger` (:mdicon:`facebook-messenger`)
  * :const:`tlp.TulipMaterialDesignIcons.Factory` (:mdicon:`factory`)
  * :const:`tlp.TulipMaterialDesignIcons.Fan` (:mdicon:`fan`)
  * :const:`tlp.TulipMaterialDesignIcons.FastForward` (:mdicon:`fast-forward`)
  * :const:`tlp.TulipMaterialDesignIcons.FastForwardOutline` (:mdicon:`fast-forward-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Fax` (:mdicon:`fax`)
  * :const:`tlp.TulipMaterialDesignIcons.Feather` (:mdicon:`feather`)
  * :const:`tlp.TulipMaterialDesignIcons.Ferry` (:mdicon:`ferry`)
  * :const:`tlp.TulipMaterialDesignIcons.File` (:mdicon:`file`)
  * :const:`tlp.TulipMaterialDesignIcons.FileAccount` (:mdicon:`file-account`)
  * :const:`tlp.TulipMaterialDesignIcons.FileChart` (:mdicon:`file-chart`)
  * :const:`tlp.TulipMaterialDesignIcons.FileCheck` (:mdicon:`file-check`)
  * :const:`tlp.TulipMaterialDesignIcons.FileCloud` (:mdicon:`file-cloud`)
  * :const:`tlp.TulipMaterialDesignIcons.FileDelimited` (:mdicon:`file-delimited`)
  * :const:`tlp.TulipMaterialDesignIcons.FileDocument` (:mdicon:`file-document`)
  * :const:`tlp.TulipMaterialDesignIcons.FileDocumentBox` (:mdicon:`file-document-box`)
  * :const:`tlp.TulipMaterialDesignIcons.FileExcel` (:mdicon:`file-excel`)
  * :const:`tlp.TulipMaterialDesignIcons.FileExcelBox` (:mdicon:`file-excel-box`)
  * :const:`tlp.TulipMaterialDesignIcons.FileExport` (:mdicon:`file-export`)
  * :const:`tlp.TulipMaterialDesignIcons.FileFind` (:mdicon:`file-find`)
  * :const:`tlp.TulipMaterialDesignIcons.FileHidden` (:mdicon:`file-hidden`)
  * :const:`tlp.TulipMaterialDesignIcons.FileImage` (:mdicon:`file-image`)
  * :const:`tlp.TulipMaterialDesignIcons.FileImport` (:mdicon:`file-import`)
  * :const:`tlp.TulipMaterialDesignIcons.FileLock` (:mdicon:`file-lock`)
  * :const:`tlp.TulipMaterialDesignIcons.FileMultiple` (:mdicon:`file-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.FileMusic` (:mdicon:`file-music`)
  * :const:`tlp.TulipMaterialDesignIcons.FileOutline` (:mdicon:`file-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.FilePdf` (:mdicon:`file-pdf`)
  * :const:`tlp.TulipMaterialDesignIcons.FilePdfBox` (:mdicon:`file-pdf-box`)
  * :const:`tlp.TulipMaterialDesignIcons.FilePlus` (:mdicon:`file-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.FilePowerpoint` (:mdicon:`file-powerpoint`)
  * :const:`tlp.TulipMaterialDesignIcons.FilePowerpointBox` (:mdicon:`file-powerpoint-box`)
  * :const:`tlp.TulipMaterialDesignIcons.FilePresentationBox` (:mdicon:`file-presentation-box`)
  * :const:`tlp.TulipMaterialDesignIcons.FileRestore` (:mdicon:`file-restore`)
  * :const:`tlp.TulipMaterialDesignIcons.FileSend` (:mdicon:`file-send`)
  * :const:`tlp.TulipMaterialDesignIcons.FileTree` (:mdicon:`file-tree`)
  * :const:`tlp.TulipMaterialDesignIcons.FileVideo` (:mdicon:`file-video`)
  * :const:`tlp.TulipMaterialDesignIcons.FileWord` (:mdicon:`file-word`)
  * :const:`tlp.TulipMaterialDesignIcons.FileWordBox` (:mdicon:`file-word-box`)
  * :const:`tlp.TulipMaterialDesignIcons.FileXml` (:mdicon:`file-xml`)
  * :const:`tlp.TulipMaterialDesignIcons.Film` (:mdicon:`film`)
  * :const:`tlp.TulipMaterialDesignIcons.Filmstrip` (:mdicon:`filmstrip`)
  * :const:`tlp.TulipMaterialDesignIcons.FilmstripOff` (:mdicon:`filmstrip-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Filter` (:mdicon:`filter`)
  * :const:`tlp.TulipMaterialDesignIcons.FilterOutline` (:mdicon:`filter-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.FilterRemove` (:mdicon:`filter-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.FilterRemoveOutline` (:mdicon:`filter-remove-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.FilterVariant` (:mdicon:`filter-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.FindReplace` (:mdicon:`find-replace`)
  * :const:`tlp.TulipMaterialDesignIcons.Fingerprint` (:mdicon:`fingerprint`)
  * :const:`tlp.TulipMaterialDesignIcons.Fire` (:mdicon:`fire`)
  * :const:`tlp.TulipMaterialDesignIcons.Firefox` (:mdicon:`firefox`)
  * :const:`tlp.TulipMaterialDesignIcons.Fish` (:mdicon:`fish`)
  * :const:`tlp.TulipMaterialDesignIcons.Flag` (:mdicon:`flag`)
  * :const:`tlp.TulipMaterialDesignIcons.FlagCheckered` (:mdicon:`flag-checkered`)
  * :const:`tlp.TulipMaterialDesignIcons.FlagOutline` (:mdicon:`flag-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.FlagOutlineVariant` (:mdicon:`flag-outline-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.FlagTriangle` (:mdicon:`flag-triangle`)
  * :const:`tlp.TulipMaterialDesignIcons.FlagVariant` (:mdicon:`flag-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Flash` (:mdicon:`flash`)
  * :const:`tlp.TulipMaterialDesignIcons.FlashAuto` (:mdicon:`flash-auto`)
  * :const:`tlp.TulipMaterialDesignIcons.FlashOff` (:mdicon:`flash-off`)
  * :const:`tlp.TulipMaterialDesignIcons.FlashOutline` (:mdicon:`flash-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.FlashRedEye` (:mdicon:`flash-red-eye`)
  * :const:`tlp.TulipMaterialDesignIcons.Flashlight` (:mdicon:`flashlight`)
  * :const:`tlp.TulipMaterialDesignIcons.FlashlightOff` (:mdicon:`flashlight-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Flask` (:mdicon:`flask`)
  * :const:`tlp.TulipMaterialDesignIcons.FlaskEmpty` (:mdicon:`flask-empty`)
  * :const:`tlp.TulipMaterialDesignIcons.FlaskEmptyOutline` (:mdicon:`flask-empty-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.FlaskOutline` (:mdicon:`flask-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Flattr` (:mdicon:`flattr`)
  * :const:`tlp.TulipMaterialDesignIcons.FlipToBack` (:mdicon:`flip-to-back`)
  * :const:`tlp.TulipMaterialDesignIcons.FlipToFront` (:mdicon:`flip-to-front`)
  * :const:`tlp.TulipMaterialDesignIcons.Floppy` (:mdicon:`floppy`)
  * :const:`tlp.TulipMaterialDesignIcons.Flower` (:mdicon:`flower`)
  * :const:`tlp.TulipMaterialDesignIcons.Folder` (:mdicon:`folder`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderAccount` (:mdicon:`folder-account`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderDownload` (:mdicon:`folder-download`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderGoogleDrive` (:mdicon:`folder-google-drive`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderImage` (:mdicon:`folder-image`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderLock` (:mdicon:`folder-lock`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderLockOpen` (:mdicon:`folder-lock-open`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderMove` (:mdicon:`folder-move`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderMultiple` (:mdicon:`folder-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderMultipleImage` (:mdicon:`folder-multiple-image`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderMultipleOutline` (:mdicon:`folder-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderOpen` (:mdicon:`folder-open`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderOutline` (:mdicon:`folder-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderPlus` (:mdicon:`folder-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderRemove` (:mdicon:`folder-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderStar` (:mdicon:`folder-star`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderUpload` (:mdicon:`folder-upload`)
  * :const:`tlp.TulipMaterialDesignIcons.FontAwesome` (:mdicon:`font-awesome`)
  * :const:`tlp.TulipMaterialDesignIcons.Food` (:mdicon:`food`)
  * :const:`tlp.TulipMaterialDesignIcons.FoodApple` (:mdicon:`food-apple`)
  * :const:`tlp.TulipMaterialDesignIcons.FoodForkDrink` (:mdicon:`food-fork-drink`)
  * :const:`tlp.TulipMaterialDesignIcons.FoodOff` (:mdicon:`food-off`)
  * :const:`tlp.TulipMaterialDesignIcons.FoodVariant` (:mdicon:`food-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Football` (:mdicon:`football`)
  * :const:`tlp.TulipMaterialDesignIcons.FootballAustralian` (:mdicon:`football-australian`)
  * :const:`tlp.TulipMaterialDesignIcons.FootballHelmet` (:mdicon:`football-helmet`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignBottom` (:mdicon:`format-align-bottom`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignCenter` (:mdicon:`format-align-center`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignJustify` (:mdicon:`format-align-justify`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignLeft` (:mdicon:`format-align-left`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignMiddle` (:mdicon:`format-align-middle`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignRight` (:mdicon:`format-align-right`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignTop` (:mdicon:`format-align-top`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAnnotationPlus` (:mdicon:`format-annotation-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatBold` (:mdicon:`format-bold`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatClear` (:mdicon:`format-clear`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatColorFill` (:mdicon:`format-color-fill`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatColorText` (:mdicon:`format-color-text`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatFloatCenter` (:mdicon:`format-float-center`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatFloatLeft` (:mdicon:`format-float-left`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatFloatNone` (:mdicon:`format-float-none`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatFloatRight` (:mdicon:`format-float-right`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatFont` (:mdicon:`format-font`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHeader1` (:mdicon:`format-header-1`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHeader2` (:mdicon:`format-header-2`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHeader3` (:mdicon:`format-header-3`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHeader4` (:mdicon:`format-header-4`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHeader5` (:mdicon:`format-header-5`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHeader6` (:mdicon:`format-header-6`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHeaderDecrease` (:mdicon:`format-header-decrease`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHeaderEqual` (:mdicon:`format-header-equal`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHeaderIncrease` (:mdicon:`format-header-increase`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHeaderPound` (:mdicon:`format-header-pound`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHorizontalAlignCenter` (:mdicon:`format-horizontal-align-center`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHorizontalAlignLeft` (:mdicon:`format-horizontal-align-left`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatHorizontalAlignRight` (:mdicon:`format-horizontal-align-right`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatIndentDecrease` (:mdicon:`format-indent-decrease`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatIndentIncrease` (:mdicon:`format-indent-increase`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatItalic` (:mdicon:`format-italic`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatLineSpacing` (:mdicon:`format-line-spacing`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatLineStyle` (:mdicon:`format-line-style`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatLineWeight` (:mdicon:`format-line-weight`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatListBulleted` (:mdicon:`format-list-bulleted`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatListBulletedType` (:mdicon:`format-list-bulleted-type`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatListChecks` (:mdicon:`format-list-checks`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatListNumbers` (:mdicon:`format-list-numbers`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatPageBreak` (:mdicon:`format-page-break`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatPaint` (:mdicon:`format-paint`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatParagraph` (:mdicon:`format-paragraph`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatPilcrow` (:mdicon:`format-pilcrow`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatQuoteClose` (:mdicon:`format-quote-close`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatQuoteOpen` (:mdicon:`format-quote-open`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatRotate90` (:mdicon:`format-rotate-90`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatSection` (:mdicon:`format-section`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatSize` (:mdicon:`format-size`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatStrikethrough` (:mdicon:`format-strikethrough`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatStrikethroughVariant` (:mdicon:`format-strikethrough-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatSubscript` (:mdicon:`format-subscript`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatSuperscript` (:mdicon:`format-superscript`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatText` (:mdicon:`format-text`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatTextdirectionLToR` (:mdicon:`format-textdirection-l-to-r`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatTextdirectionRToL` (:mdicon:`format-textdirection-r-to-l`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatTitle` (:mdicon:`format-title`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatUnderline` (:mdicon:`format-underline`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatVerticalAlignBottom` (:mdicon:`format-vertical-align-bottom`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatVerticalAlignCenter` (:mdicon:`format-vertical-align-center`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatVerticalAlignTop` (:mdicon:`format-vertical-align-top`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatWrapInline` (:mdicon:`format-wrap-inline`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatWrapSquare` (:mdicon:`format-wrap-square`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatWrapTight` (:mdicon:`format-wrap-tight`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatWrapTopBottom` (:mdicon:`format-wrap-top-bottom`)
  * :const:`tlp.TulipMaterialDesignIcons.Forum` (:mdicon:`forum`)
  * :const:`tlp.TulipMaterialDesignIcons.Forward` (:mdicon:`forward`)
  * :const:`tlp.TulipMaterialDesignIcons.Foursquare` (:mdicon:`foursquare`)
  * :const:`tlp.TulipMaterialDesignIcons.Fridge` (:mdicon:`fridge`)
  * :const:`tlp.TulipMaterialDesignIcons.FridgeFilled` (:mdicon:`fridge-filled`)
  * :const:`tlp.TulipMaterialDesignIcons.FridgeFilledBottom` (:mdicon:`fridge-filled-bottom`)
  * :const:`tlp.TulipMaterialDesignIcons.FridgeFilledTop` (:mdicon:`fridge-filled-top`)
  * :const:`tlp.TulipMaterialDesignIcons.Fullscreen` (:mdicon:`fullscreen`)
  * :const:`tlp.TulipMaterialDesignIcons.FullscreenExit` (:mdicon:`fullscreen-exit`)
  * :const:`tlp.TulipMaterialDesignIcons.Function` (:mdicon:`function`)
  * :const:`tlp.TulipMaterialDesignIcons.Gamepad` (:mdicon:`gamepad`)
  * :const:`tlp.TulipMaterialDesignIcons.GamepadVariant` (:mdicon:`gamepad-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Garage` (:mdicon:`garage`)
  * :const:`tlp.TulipMaterialDesignIcons.GarageOpen` (:mdicon:`garage-open`)
  * :const:`tlp.TulipMaterialDesignIcons.GasCylinder` (:mdicon:`gas-cylinder`)
  * :const:`tlp.TulipMaterialDesignIcons.GasStation` (:mdicon:`gas-station`)
  * :const:`tlp.TulipMaterialDesignIcons.Gate` (:mdicon:`gate`)
  * :const:`tlp.TulipMaterialDesignIcons.Gauge` (:mdicon:`gauge`)
  * :const:`tlp.TulipMaterialDesignIcons.Gavel` (:mdicon:`gavel`)
  * :const:`tlp.TulipMaterialDesignIcons.GenderFemale` (:mdicon:`gender-female`)
  * :const:`tlp.TulipMaterialDesignIcons.GenderMale` (:mdicon:`gender-male`)
  * :const:`tlp.TulipMaterialDesignIcons.GenderMaleFemale` (:mdicon:`gender-male-female`)
  * :const:`tlp.TulipMaterialDesignIcons.GenderTransgender` (:mdicon:`gender-transgender`)
  * :const:`tlp.TulipMaterialDesignIcons.GestureDoubleTap` (:mdicon:`gesture-double-tap`)
  * :const:`tlp.TulipMaterialDesignIcons.GestureSwipeDown` (:mdicon:`gesture-swipe-down`)
  * :const:`tlp.TulipMaterialDesignIcons.GestureSwipeLeft` (:mdicon:`gesture-swipe-left`)
  * :const:`tlp.TulipMaterialDesignIcons.GestureSwipeRight` (:mdicon:`gesture-swipe-right`)
  * :const:`tlp.TulipMaterialDesignIcons.GestureSwipeUp` (:mdicon:`gesture-swipe-up`)
  * :const:`tlp.TulipMaterialDesignIcons.GestureTap` (:mdicon:`gesture-tap`)
  * :const:`tlp.TulipMaterialDesignIcons.GestureTwoDoubleTap` (:mdicon:`gesture-two-double-tap`)
  * :const:`tlp.TulipMaterialDesignIcons.GestureTwoTap` (:mdicon:`gesture-two-tap`)
  * :const:`tlp.TulipMaterialDesignIcons.Ghost` (:mdicon:`ghost`)
  * :const:`tlp.TulipMaterialDesignIcons.Gift` (:mdicon:`gift`)
  * :const:`tlp.TulipMaterialDesignIcons.Git` (:mdicon:`git`)
  * :const:`tlp.TulipMaterialDesignIcons.GithubBox` (:mdicon:`github-box`)
  * :const:`tlp.TulipMaterialDesignIcons.GithubCircle` (:mdicon:`github-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.GithubFace` (:mdicon:`github-face`)
  * :const:`tlp.TulipMaterialDesignIcons.GlassFlute` (:mdicon:`glass-flute`)
  * :const:`tlp.TulipMaterialDesignIcons.GlassMug` (:mdicon:`glass-mug`)
  * :const:`tlp.TulipMaterialDesignIcons.GlassStange` (:mdicon:`glass-stange`)
  * :const:`tlp.TulipMaterialDesignIcons.GlassTulip` (:mdicon:`glass-tulip`)
  * :const:`tlp.TulipMaterialDesignIcons.Glassdoor` (:mdicon:`glassdoor`)
  * :const:`tlp.TulipMaterialDesignIcons.Glasses` (:mdicon:`glasses`)
  * :const:`tlp.TulipMaterialDesignIcons.Gmail` (:mdicon:`gmail`)
  * :const:`tlp.TulipMaterialDesignIcons.Gnome` (:mdicon:`gnome`)
  * :const:`tlp.TulipMaterialDesignIcons.Gondola` (:mdicon:`gondola`)
  * :const:`tlp.TulipMaterialDesignIcons.Google` (:mdicon:`google`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleCardboard` (:mdicon:`google-cardboard`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleChrome` (:mdicon:`google-chrome`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleCircles` (:mdicon:`google-circles`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleCirclesCommunities` (:mdicon:`google-circles-communities`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleCirclesExtended` (:mdicon:`google-circles-extended`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleCirclesGroup` (:mdicon:`google-circles-group`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleController` (:mdicon:`google-controller`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleControllerOff` (:mdicon:`google-controller-off`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleDrive` (:mdicon:`google-drive`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleEarth` (:mdicon:`google-earth`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleGlass` (:mdicon:`google-glass`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleKeep` (:mdicon:`google-keep`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleMaps` (:mdicon:`google-maps`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleNearby` (:mdicon:`google-nearby`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePages` (:mdicon:`google-pages`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePhotos` (:mdicon:`google-photos`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePhysicalWeb` (:mdicon:`google-physical-web`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePlay` (:mdicon:`google-play`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePlus` (:mdicon:`google-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePlusBox` (:mdicon:`google-plus-box`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleTranslate` (:mdicon:`google-translate`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleWallet` (:mdicon:`google-wallet`)
  * :const:`tlp.TulipMaterialDesignIcons.Gradient` (:mdicon:`gradient`)
  * :const:`tlp.TulipMaterialDesignIcons.GreasePencil` (:mdicon:`grease-pencil`)
  * :const:`tlp.TulipMaterialDesignIcons.Grid` (:mdicon:`grid`)
  * :const:`tlp.TulipMaterialDesignIcons.GridLarge` (:mdicon:`grid-large`)
  * :const:`tlp.TulipMaterialDesignIcons.GridOff` (:mdicon:`grid-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Group` (:mdicon:`group`)
  * :const:`tlp.TulipMaterialDesignIcons.GuitarAcoustic` (:mdicon:`guitar-acoustic`)
  * :const:`tlp.TulipMaterialDesignIcons.GuitarElectric` (:mdicon:`guitar-electric`)
  * :const:`tlp.TulipMaterialDesignIcons.GuitarPick` (:mdicon:`guitar-pick`)
  * :const:`tlp.TulipMaterialDesignIcons.GuitarPickOutline` (:mdicon:`guitar-pick-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Hackernews` (:mdicon:`hackernews`)
  * :const:`tlp.TulipMaterialDesignIcons.Hamburger` (:mdicon:`hamburger`)
  * :const:`tlp.TulipMaterialDesignIcons.HandPointingRight` (:mdicon:`hand-pointing-right`)
  * :const:`tlp.TulipMaterialDesignIcons.Hanger` (:mdicon:`hanger`)
  * :const:`tlp.TulipMaterialDesignIcons.Hangouts` (:mdicon:`hangouts`)
  * :const:`tlp.TulipMaterialDesignIcons.Harddisk` (:mdicon:`harddisk`)
  * :const:`tlp.TulipMaterialDesignIcons.Headphones` (:mdicon:`headphones`)
  * :const:`tlp.TulipMaterialDesignIcons.HeadphonesBox` (:mdicon:`headphones-box`)
  * :const:`tlp.TulipMaterialDesignIcons.HeadphonesSettings` (:mdicon:`headphones-settings`)
  * :const:`tlp.TulipMaterialDesignIcons.Headset` (:mdicon:`headset`)
  * :const:`tlp.TulipMaterialDesignIcons.HeadsetDock` (:mdicon:`headset-dock`)
  * :const:`tlp.TulipMaterialDesignIcons.HeadsetOff` (:mdicon:`headset-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Heart` (:mdicon:`heart`)
  * :const:`tlp.TulipMaterialDesignIcons.HeartBox` (:mdicon:`heart-box`)
  * :const:`tlp.TulipMaterialDesignIcons.HeartBoxOutline` (:mdicon:`heart-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.HeartBroken` (:mdicon:`heart-broken`)
  * :const:`tlp.TulipMaterialDesignIcons.HeartHalf` (:mdicon:`heart-half`)
  * :const:`tlp.TulipMaterialDesignIcons.HeartHalfFull` (:mdicon:`heart-half-full`)
  * :const:`tlp.TulipMaterialDesignIcons.HeartHalfOutline` (:mdicon:`heart-half-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.HeartOff` (:mdicon:`heart-off`)
  * :const:`tlp.TulipMaterialDesignIcons.HeartOutline` (:mdicon:`heart-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.HeartPulse` (:mdicon:`heart-pulse`)
  * :const:`tlp.TulipMaterialDesignIcons.Help` (:mdicon:`help`)
  * :const:`tlp.TulipMaterialDesignIcons.HelpBox` (:mdicon:`help-box`)
  * :const:`tlp.TulipMaterialDesignIcons.HelpCircle` (:mdicon:`help-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.HelpCircleOutline` (:mdicon:`help-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.HelpNetwork` (:mdicon:`help-network`)
  * :const:`tlp.TulipMaterialDesignIcons.Hexagon` (:mdicon:`hexagon`)
  * :const:`tlp.TulipMaterialDesignIcons.HexagonMultiple` (:mdicon:`hexagon-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.HexagonOutline` (:mdicon:`hexagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Highway` (:mdicon:`highway`)
  * :const:`tlp.TulipMaterialDesignIcons.History` (:mdicon:`history`)
  * :const:`tlp.TulipMaterialDesignIcons.Hololens` (:mdicon:`hololens`)
  * :const:`tlp.TulipMaterialDesignIcons.Home` (:mdicon:`home`)
  * :const:`tlp.TulipMaterialDesignIcons.HomeMapMarker` (:mdicon:`home-map-marker`)
  * :const:`tlp.TulipMaterialDesignIcons.HomeModern` (:mdicon:`home-modern`)
  * :const:`tlp.TulipMaterialDesignIcons.HomeOutline` (:mdicon:`home-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.HomeVariant` (:mdicon:`home-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Hook` (:mdicon:`hook`)
  * :const:`tlp.TulipMaterialDesignIcons.HookOff` (:mdicon:`hook-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Hops` (:mdicon:`hops`)
  * :const:`tlp.TulipMaterialDesignIcons.Hospital` (:mdicon:`hospital`)
  * :const:`tlp.TulipMaterialDesignIcons.HospitalBuilding` (:mdicon:`hospital-building`)
  * :const:`tlp.TulipMaterialDesignIcons.HospitalMarker` (:mdicon:`hospital-marker`)
  * :const:`tlp.TulipMaterialDesignIcons.Hotel` (:mdicon:`hotel`)
  * :const:`tlp.TulipMaterialDesignIcons.Houzz` (:mdicon:`houzz`)
  * :const:`tlp.TulipMaterialDesignIcons.HouzzBox` (:mdicon:`houzz-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Human` (:mdicon:`human`)
  * :const:`tlp.TulipMaterialDesignIcons.HumanChild` (:mdicon:`human-child`)
  * :const:`tlp.TulipMaterialDesignIcons.HumanFemale` (:mdicon:`human-female`)
  * :const:`tlp.TulipMaterialDesignIcons.HumanGreeting` (:mdicon:`human-greeting`)
  * :const:`tlp.TulipMaterialDesignIcons.HumanHandsdown` (:mdicon:`human-handsdown`)
  * :const:`tlp.TulipMaterialDesignIcons.HumanHandsup` (:mdicon:`human-handsup`)
  * :const:`tlp.TulipMaterialDesignIcons.HumanMale` (:mdicon:`human-male`)
  * :const:`tlp.TulipMaterialDesignIcons.HumanMaleFemale` (:mdicon:`human-male-female`)
  * :const:`tlp.TulipMaterialDesignIcons.HumanPregnant` (:mdicon:`human-pregnant`)
  * :const:`tlp.TulipMaterialDesignIcons.HumbleBundle` (:mdicon:`humble-bundle`)
  * :const:`tlp.TulipMaterialDesignIcons.Image` (:mdicon:`image`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageAlbum` (:mdicon:`image-album`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageArea` (:mdicon:`image-area`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageAreaClose` (:mdicon:`image-area-close`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageBroken` (:mdicon:`image-broken`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageBrokenVariant` (:mdicon:`image-broken-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageFilter` (:mdicon:`image-filter`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageFilterBlackWhite` (:mdicon:`image-filter-black-white`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageFilterCenterFocus` (:mdicon:`image-filter-center-focus`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageFilterCenterFocusWeak` (:mdicon:`image-filter-center-focus-weak`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageFilterDrama` (:mdicon:`image-filter-drama`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageFilterFrames` (:mdicon:`image-filter-frames`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageFilterHdr` (:mdicon:`image-filter-hdr`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageFilterNone` (:mdicon:`image-filter-none`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageFilterTiltShift` (:mdicon:`image-filter-tilt-shift`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageFilterVintage` (:mdicon:`image-filter-vintage`)
  * :const:`tlp.TulipMaterialDesignIcons.ImageMultiple` (:mdicon:`image-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.Import` (:mdicon:`import`)
  * :const:`tlp.TulipMaterialDesignIcons.Inbox` (:mdicon:`inbox`)
  * :const:`tlp.TulipMaterialDesignIcons.InboxArrowDown` (:mdicon:`inbox-arrow-down`)
  * :const:`tlp.TulipMaterialDesignIcons.InboxArrowUp` (:mdicon:`inbox-arrow-up`)
  * :const:`tlp.TulipMaterialDesignIcons.Incognito` (:mdicon:`incognito`)
  * :const:`tlp.TulipMaterialDesignIcons.Infinity` (:mdicon:`infinity`)
  * :const:`tlp.TulipMaterialDesignIcons.Information` (:mdicon:`information`)
  * :const:`tlp.TulipMaterialDesignIcons.InformationOutline` (:mdicon:`information-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.InformationVariant` (:mdicon:`information-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Instagram` (:mdicon:`instagram`)
  * :const:`tlp.TulipMaterialDesignIcons.Instapaper` (:mdicon:`instapaper`)
  * :const:`tlp.TulipMaterialDesignIcons.InternetExplorer` (:mdicon:`internet-explorer`)
  * :const:`tlp.TulipMaterialDesignIcons.InvertColors` (:mdicon:`invert-colors`)
  * :const:`tlp.TulipMaterialDesignIcons.Itunes` (:mdicon:`itunes`)
  * :const:`tlp.TulipMaterialDesignIcons.Jeepney` (:mdicon:`jeepney`)
  * :const:`tlp.TulipMaterialDesignIcons.Jira` (:mdicon:`jira`)
  * :const:`tlp.TulipMaterialDesignIcons.Jsfiddle` (:mdicon:`jsfiddle`)
  * :const:`tlp.TulipMaterialDesignIcons.Json` (:mdicon:`json`)
  * :const:`tlp.TulipMaterialDesignIcons.Keg` (:mdicon:`keg`)
  * :const:`tlp.TulipMaterialDesignIcons.Kettle` (:mdicon:`kettle`)
  * :const:`tlp.TulipMaterialDesignIcons.Key` (:mdicon:`key`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyChange` (:mdicon:`key-change`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyMinus` (:mdicon:`key-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyPlus` (:mdicon:`key-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyRemove` (:mdicon:`key-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyVariant` (:mdicon:`key-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Keyboard` (:mdicon:`keyboard`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyboardBackspace` (:mdicon:`keyboard-backspace`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyboardCaps` (:mdicon:`keyboard-caps`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyboardClose` (:mdicon:`keyboard-close`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyboardOff` (:mdicon:`keyboard-off`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyboardReturn` (:mdicon:`keyboard-return`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyboardTab` (:mdicon:`keyboard-tab`)
  * :const:`tlp.TulipMaterialDesignIcons.KeyboardVariant` (:mdicon:`keyboard-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Kickstarter` (:mdicon:`kickstarter`)
  * :const:`tlp.TulipMaterialDesignIcons.Kodi` (:mdicon:`kodi`)
  * :const:`tlp.TulipMaterialDesignIcons.Label` (:mdicon:`label`)
  * :const:`tlp.TulipMaterialDesignIcons.LabelOutline` (:mdicon:`label-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Lambda` (:mdicon:`lambda`)
  * :const:`tlp.TulipMaterialDesignIcons.Lamp` (:mdicon:`lamp`)
  * :const:`tlp.TulipMaterialDesignIcons.Lan` (:mdicon:`lan`)
  * :const:`tlp.TulipMaterialDesignIcons.LanConnect` (:mdicon:`lan-connect`)
  * :const:`tlp.TulipMaterialDesignIcons.LanDisconnect` (:mdicon:`lan-disconnect`)
  * :const:`tlp.TulipMaterialDesignIcons.LanPending` (:mdicon:`lan-pending`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguageC` (:mdicon:`language-c`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguageCpp` (:mdicon:`language-cpp`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguageCsharp` (:mdicon:`language-csharp`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguageCss3` (:mdicon:`language-css3`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguageHtml5` (:mdicon:`language-html5`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguageJavascript` (:mdicon:`language-javascript`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguagePhp` (:mdicon:`language-php`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguagePython` (:mdicon:`language-python`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguagePythonText` (:mdicon:`language-python-text`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguageSwift` (:mdicon:`language-swift`)
  * :const:`tlp.TulipMaterialDesignIcons.LanguageTypescript` (:mdicon:`language-typescript`)
  * :const:`tlp.TulipMaterialDesignIcons.Laptop` (:mdicon:`laptop`)
  * :const:`tlp.TulipMaterialDesignIcons.LaptopChromebook` (:mdicon:`laptop-chromebook`)
  * :const:`tlp.TulipMaterialDesignIcons.LaptopMac` (:mdicon:`laptop-mac`)
  * :const:`tlp.TulipMaterialDesignIcons.LaptopOff` (:mdicon:`laptop-off`)
  * :const:`tlp.TulipMaterialDesignIcons.LaptopWindows` (:mdicon:`laptop-windows`)
  * :const:`tlp.TulipMaterialDesignIcons.Lastfm` (:mdicon:`lastfm`)
  * :const:`tlp.TulipMaterialDesignIcons.Launch` (:mdicon:`launch`)
  * :const:`tlp.TulipMaterialDesignIcons.Layers` (:mdicon:`layers`)
  * :const:`tlp.TulipMaterialDesignIcons.LayersOff` (:mdicon:`layers-off`)
  * :const:`tlp.TulipMaterialDesignIcons.LeadPencil` (:mdicon:`lead-pencil`)
  * :const:`tlp.TulipMaterialDesignIcons.Leaf` (:mdicon:`leaf`)
  * :const:`tlp.TulipMaterialDesignIcons.LedOff` (:mdicon:`led-off`)
  * :const:`tlp.TulipMaterialDesignIcons.LedOn` (:mdicon:`led-on`)
  * :const:`tlp.TulipMaterialDesignIcons.LedOutline` (:mdicon:`led-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.LedVariantOff` (:mdicon:`led-variant-off`)
  * :const:`tlp.TulipMaterialDesignIcons.LedVariantOn` (:mdicon:`led-variant-on`)
  * :const:`tlp.TulipMaterialDesignIcons.LedVariantOutline` (:mdicon:`led-variant-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Library` (:mdicon:`library`)
  * :const:`tlp.TulipMaterialDesignIcons.LibraryBooks` (:mdicon:`library-books`)
  * :const:`tlp.TulipMaterialDesignIcons.LibraryMusic` (:mdicon:`library-music`)
  * :const:`tlp.TulipMaterialDesignIcons.LibraryPlus` (:mdicon:`library-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.Lightbulb` (:mdicon:`lightbulb`)
  * :const:`tlp.TulipMaterialDesignIcons.LightbulbOn` (:mdicon:`lightbulb-on`)
  * :const:`tlp.TulipMaterialDesignIcons.LightbulbOnOutline` (:mdicon:`lightbulb-on-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.LightbulbOutline` (:mdicon:`lightbulb-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Link` (:mdicon:`link`)
  * :const:`tlp.TulipMaterialDesignIcons.LinkOff` (:mdicon:`link-off`)
  * :const:`tlp.TulipMaterialDesignIcons.LinkVariant` (:mdicon:`link-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.LinkVariantOff` (:mdicon:`link-variant-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Linkedin` (:mdicon:`linkedin`)
  * :const:`tlp.TulipMaterialDesignIcons.LinkedinBox` (:mdicon:`linkedin-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Linux` (:mdicon:`linux`)
  * :const:`tlp.TulipMaterialDesignIcons.Loading` (:mdicon:`loading`)
  * :const:`tlp.TulipMaterialDesignIcons.Lock` (:mdicon:`lock`)
  * :const:`tlp.TulipMaterialDesignIcons.LockOpen` (:mdicon:`lock-open`)
  * :const:`tlp.TulipMaterialDesignIcons.LockOpenOutline` (:mdicon:`lock-open-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.LockOutline` (:mdicon:`lock-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.LockPattern` (:mdicon:`lock-pattern`)
  * :const:`tlp.TulipMaterialDesignIcons.LockPlus` (:mdicon:`lock-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.LockReset` (:mdicon:`lock-reset`)
  * :const:`tlp.TulipMaterialDesignIcons.Login` (:mdicon:`login`)
  * :const:`tlp.TulipMaterialDesignIcons.LoginVariant` (:mdicon:`login-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Logout` (:mdicon:`logout`)
  * :const:`tlp.TulipMaterialDesignIcons.LogoutVariant` (:mdicon:`logout-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Looks` (:mdicon:`looks`)
  * :const:`tlp.TulipMaterialDesignIcons.Loop` (:mdicon:`loop`)
  * :const:`tlp.TulipMaterialDesignIcons.Loupe` (:mdicon:`loupe`)
  * :const:`tlp.TulipMaterialDesignIcons.Lumx` (:mdicon:`lumx`)
  * :const:`tlp.TulipMaterialDesignIcons.Magnet` (:mdicon:`magnet`)
  * :const:`tlp.TulipMaterialDesignIcons.MagnetOn` (:mdicon:`magnet-on`)
  * :const:`tlp.TulipMaterialDesignIcons.Magnify` (:mdicon:`magnify`)
  * :const:`tlp.TulipMaterialDesignIcons.MagnifyMinus` (:mdicon:`magnify-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.MagnifyMinusOutline` (:mdicon:`magnify-minus-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.MagnifyPlus` (:mdicon:`magnify-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.MagnifyPlusOutline` (:mdicon:`magnify-plus-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.MailRu` (:mdicon:`mail-ru`)
  * :const:`tlp.TulipMaterialDesignIcons.Mailbox` (:mdicon:`mailbox`)
  * :const:`tlp.TulipMaterialDesignIcons.Map` (:mdicon:`map`)
  * :const:`tlp.TulipMaterialDesignIcons.MapMarker` (:mdicon:`map-marker`)
  * :const:`tlp.TulipMaterialDesignIcons.MapMarkerCircle` (:mdicon:`map-marker-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.MapMarkerMinus` (:mdicon:`map-marker-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.MapMarkerMultiple` (:mdicon:`map-marker-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.MapMarkerOff` (:mdicon:`map-marker-off`)
  * :const:`tlp.TulipMaterialDesignIcons.MapMarkerPlus` (:mdicon:`map-marker-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.MapMarkerRadius` (:mdicon:`map-marker-radius`)
  * :const:`tlp.TulipMaterialDesignIcons.Margin` (:mdicon:`margin`)
  * :const:`tlp.TulipMaterialDesignIcons.Markdown` (:mdicon:`markdown`)
  * :const:`tlp.TulipMaterialDesignIcons.Marker` (:mdicon:`marker`)
  * :const:`tlp.TulipMaterialDesignIcons.MarkerCheck` (:mdicon:`marker-check`)
  * :const:`tlp.TulipMaterialDesignIcons.Martini` (:mdicon:`martini`)
  * :const:`tlp.TulipMaterialDesignIcons.MaterialUi` (:mdicon:`material-ui`)
  * :const:`tlp.TulipMaterialDesignIcons.MathCompass` (:mdicon:`math-compass`)
  * :const:`tlp.TulipMaterialDesignIcons.Matrix` (:mdicon:`matrix`)
  * :const:`tlp.TulipMaterialDesignIcons.Maxcdn` (:mdicon:`maxcdn`)
  * :const:`tlp.TulipMaterialDesignIcons.MedicalBag` (:mdicon:`medical-bag`)
  * :const:`tlp.TulipMaterialDesignIcons.Medium` (:mdicon:`medium`)
  * :const:`tlp.TulipMaterialDesignIcons.Memory` (:mdicon:`memory`)
  * :const:`tlp.TulipMaterialDesignIcons.Menu` (:mdicon:`menu`)
  * :const:`tlp.TulipMaterialDesignIcons.MenuDown` (:mdicon:`menu-down`)
  * :const:`tlp.TulipMaterialDesignIcons.MenuDownOutline` (:mdicon:`menu-down-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.MenuLeft` (:mdicon:`menu-left`)
  * :const:`tlp.TulipMaterialDesignIcons.MenuRight` (:mdicon:`menu-right`)
  * :const:`tlp.TulipMaterialDesignIcons.MenuUp` (:mdicon:`menu-up`)
  * :const:`tlp.TulipMaterialDesignIcons.MenuUpOutline` (:mdicon:`menu-up-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Message` (:mdicon:`message`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageAlert` (:mdicon:`message-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageBulleted` (:mdicon:`message-bulleted`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageBulletedOff` (:mdicon:`message-bulleted-off`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageDraw` (:mdicon:`message-draw`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageImage` (:mdicon:`message-image`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageOutline` (:mdicon:`message-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.MessagePlus` (:mdicon:`message-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageProcessing` (:mdicon:`message-processing`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageReply` (:mdicon:`message-reply`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageReplyText` (:mdicon:`message-reply-text`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageSettings` (:mdicon:`message-settings`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageSettingsVariant` (:mdicon:`message-settings-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageText` (:mdicon:`message-text`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageTextOutline` (:mdicon:`message-text-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.MessageVideo` (:mdicon:`message-video`)
  * :const:`tlp.TulipMaterialDesignIcons.Meteor` (:mdicon:`meteor`)
  * :const:`tlp.TulipMaterialDesignIcons.Microphone` (:mdicon:`microphone`)
  * :const:`tlp.TulipMaterialDesignIcons.MicrophoneOff` (:mdicon:`microphone-off`)
  * :const:`tlp.TulipMaterialDesignIcons.MicrophoneOutline` (:mdicon:`microphone-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.MicrophoneSettings` (:mdicon:`microphone-settings`)
  * :const:`tlp.TulipMaterialDesignIcons.MicrophoneVariant` (:mdicon:`microphone-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.MicrophoneVariantOff` (:mdicon:`microphone-variant-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Microscope` (:mdicon:`microscope`)
  * :const:`tlp.TulipMaterialDesignIcons.Microsoft` (:mdicon:`microsoft`)
  * :const:`tlp.TulipMaterialDesignIcons.Minecraft` (:mdicon:`minecraft`)
  * :const:`tlp.TulipMaterialDesignIcons.Minus` (:mdicon:`minus`)
  * :const:`tlp.TulipMaterialDesignIcons.MinusBox` (:mdicon:`minus-box`)
  * :const:`tlp.TulipMaterialDesignIcons.MinusBoxOutline` (:mdicon:`minus-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.MinusCircle` (:mdicon:`minus-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.MinusCircleOutline` (:mdicon:`minus-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.MinusNetwork` (:mdicon:`minus-network`)
  * :const:`tlp.TulipMaterialDesignIcons.Mixcloud` (:mdicon:`mixcloud`)
  * :const:`tlp.TulipMaterialDesignIcons.Monitor` (:mdicon:`monitor`)
  * :const:`tlp.TulipMaterialDesignIcons.MonitorMultiple` (:mdicon:`monitor-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.More` (:mdicon:`more`)
  * :const:`tlp.TulipMaterialDesignIcons.Motorbike` (:mdicon:`motorbike`)
  * :const:`tlp.TulipMaterialDesignIcons.Mouse` (:mdicon:`mouse`)
  * :const:`tlp.TulipMaterialDesignIcons.MouseOff` (:mdicon:`mouse-off`)
  * :const:`tlp.TulipMaterialDesignIcons.MouseVariant` (:mdicon:`mouse-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.MouseVariantOff` (:mdicon:`mouse-variant-off`)
  * :const:`tlp.TulipMaterialDesignIcons.MoveResize` (:mdicon:`move-resize`)
  * :const:`tlp.TulipMaterialDesignIcons.MoveResizeVariant` (:mdicon:`move-resize-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Movie` (:mdicon:`movie`)
  * :const:`tlp.TulipMaterialDesignIcons.Multiplication` (:mdicon:`multiplication`)
  * :const:`tlp.TulipMaterialDesignIcons.MultiplicationBox` (:mdicon:`multiplication-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Music` (:mdicon:`music`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicBox` (:mdicon:`music-box`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicBoxOutline` (:mdicon:`music-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicCircle` (:mdicon:`music-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicNote` (:mdicon:`music-note`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicNoteBluetooth` (:mdicon:`music-note-bluetooth`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicNoteBluetoothOff` (:mdicon:`music-note-bluetooth-off`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicNoteEighth` (:mdicon:`music-note-eighth`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicNoteHalf` (:mdicon:`music-note-half`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicNoteOff` (:mdicon:`music-note-off`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicNoteQuarter` (:mdicon:`music-note-quarter`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicNoteSixteenth` (:mdicon:`music-note-sixteenth`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicNoteWhole` (:mdicon:`music-note-whole`)
  * :const:`tlp.TulipMaterialDesignIcons.MusicOff` (:mdicon:`music-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Nature` (:mdicon:`nature`)
  * :const:`tlp.TulipMaterialDesignIcons.NaturePeople` (:mdicon:`nature-people`)
  * :const:`tlp.TulipMaterialDesignIcons.Navigation` (:mdicon:`navigation`)
  * :const:`tlp.TulipMaterialDesignIcons.NearMe` (:mdicon:`near-me`)
  * :const:`tlp.TulipMaterialDesignIcons.Needle` (:mdicon:`needle`)
  * :const:`tlp.TulipMaterialDesignIcons.NestProtect` (:mdicon:`nest-protect`)
  * :const:`tlp.TulipMaterialDesignIcons.NestThermostat` (:mdicon:`nest-thermostat`)
  * :const:`tlp.TulipMaterialDesignIcons.Netflix` (:mdicon:`netflix`)
  * :const:`tlp.TulipMaterialDesignIcons.Network` (:mdicon:`network`)
  * :const:`tlp.TulipMaterialDesignIcons.NewBox` (:mdicon:`new-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Newspaper` (:mdicon:`newspaper`)
  * :const:`tlp.TulipMaterialDesignIcons.Nfc` (:mdicon:`nfc`)
  * :const:`tlp.TulipMaterialDesignIcons.NfcTap` (:mdicon:`nfc-tap`)
  * :const:`tlp.TulipMaterialDesignIcons.NfcVariant` (:mdicon:`nfc-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Ninja` (:mdicon:`ninja`)
  * :const:`tlp.TulipMaterialDesignIcons.Nodejs` (:mdicon:`nodejs`)
  * :const:`tlp.TulipMaterialDesignIcons.Note` (:mdicon:`note`)
  * :const:`tlp.TulipMaterialDesignIcons.NoteMultiple` (:mdicon:`note-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.NoteMultipleOutline` (:mdicon:`note-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.NoteOutline` (:mdicon:`note-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.NotePlus` (:mdicon:`note-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.NotePlusOutline` (:mdicon:`note-plus-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.NoteText` (:mdicon:`note-text`)
  * :const:`tlp.TulipMaterialDesignIcons.NotificationClearAll` (:mdicon:`notification-clear-all`)
  * :const:`tlp.TulipMaterialDesignIcons.Npm` (:mdicon:`npm`)
  * :const:`tlp.TulipMaterialDesignIcons.Nuke` (:mdicon:`nuke`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric` (:mdicon:`numeric`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric0Box` (:mdicon:`numeric-0-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric0BoxMultipleOutline` (:mdicon:`numeric-0-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric0BoxOutline` (:mdicon:`numeric-0-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric1Box` (:mdicon:`numeric-1-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric1BoxMultipleOutline` (:mdicon:`numeric-1-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric1BoxOutline` (:mdicon:`numeric-1-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric2Box` (:mdicon:`numeric-2-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric2BoxMultipleOutline` (:mdicon:`numeric-2-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric2BoxOutline` (:mdicon:`numeric-2-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric3Box` (:mdicon:`numeric-3-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric3BoxMultipleOutline` (:mdicon:`numeric-3-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric3BoxOutline` (:mdicon:`numeric-3-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric4Box` (:mdicon:`numeric-4-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric4BoxMultipleOutline` (:mdicon:`numeric-4-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric4BoxOutline` (:mdicon:`numeric-4-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric5Box` (:mdicon:`numeric-5-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric5BoxMultipleOutline` (:mdicon:`numeric-5-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric5BoxOutline` (:mdicon:`numeric-5-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric6Box` (:mdicon:`numeric-6-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric6BoxMultipleOutline` (:mdicon:`numeric-6-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric6BoxOutline` (:mdicon:`numeric-6-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric7Box` (:mdicon:`numeric-7-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric7BoxMultipleOutline` (:mdicon:`numeric-7-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric7BoxOutline` (:mdicon:`numeric-7-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric8Box` (:mdicon:`numeric-8-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric8BoxMultipleOutline` (:mdicon:`numeric-8-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric8BoxOutline` (:mdicon:`numeric-8-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric9Box` (:mdicon:`numeric-9-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric9BoxMultipleOutline` (:mdicon:`numeric-9-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric9BoxOutline` (:mdicon:`numeric-9-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric9PlusBox` (:mdicon:`numeric-9-plus-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric9PlusBoxMultipleOutline` (:mdicon:`numeric-9-plus-box-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Numeric9PlusBoxOutline` (:mdicon:`numeric-9-plus-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Nut` (:mdicon:`nut`)
  * :const:`tlp.TulipMaterialDesignIcons.Nutrition` (:mdicon:`nutrition`)
  * :const:`tlp.TulipMaterialDesignIcons.Oar` (:mdicon:`oar`)
  * :const:`tlp.TulipMaterialDesignIcons.Octagon` (:mdicon:`octagon`)
  * :const:`tlp.TulipMaterialDesignIcons.OctagonOutline` (:mdicon:`octagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Octagram` (:mdicon:`octagram`)
  * :const:`tlp.TulipMaterialDesignIcons.OctagramOutline` (:mdicon:`octagram-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Odnoklassniki` (:mdicon:`odnoklassniki`)
  * :const:`tlp.TulipMaterialDesignIcons.Office` (:mdicon:`office`)
  * :const:`tlp.TulipMaterialDesignIcons.Oil` (:mdicon:`oil`)
  * :const:`tlp.TulipMaterialDesignIcons.OilTemperature` (:mdicon:`oil-temperature`)
  * :const:`tlp.TulipMaterialDesignIcons.Omega` (:mdicon:`omega`)
  * :const:`tlp.TulipMaterialDesignIcons.Onedrive` (:mdicon:`onedrive`)
  * :const:`tlp.TulipMaterialDesignIcons.Onenote` (:mdicon:`onenote`)
  * :const:`tlp.TulipMaterialDesignIcons.Opacity` (:mdicon:`opacity`)
  * :const:`tlp.TulipMaterialDesignIcons.OpenInApp` (:mdicon:`open-in-app`)
  * :const:`tlp.TulipMaterialDesignIcons.OpenInNew` (:mdicon:`open-in-new`)
  * :const:`tlp.TulipMaterialDesignIcons.Openid` (:mdicon:`openid`)
  * :const:`tlp.TulipMaterialDesignIcons.Opera` (:mdicon:`opera`)
  * :const:`tlp.TulipMaterialDesignIcons.Orbit` (:mdicon:`orbit`)
  * :const:`tlp.TulipMaterialDesignIcons.Ornament` (:mdicon:`ornament`)
  * :const:`tlp.TulipMaterialDesignIcons.OrnamentVariant` (:mdicon:`ornament-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Owl` (:mdicon:`owl`)
  * :const:`tlp.TulipMaterialDesignIcons.Package` (:mdicon:`package`)
  * :const:`tlp.TulipMaterialDesignIcons.PackageDown` (:mdicon:`package-down`)
  * :const:`tlp.TulipMaterialDesignIcons.PackageUp` (:mdicon:`package-up`)
  * :const:`tlp.TulipMaterialDesignIcons.PackageVariant` (:mdicon:`package-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.PackageVariantClosed` (:mdicon:`package-variant-closed`)
  * :const:`tlp.TulipMaterialDesignIcons.PageFirst` (:mdicon:`page-first`)
  * :const:`tlp.TulipMaterialDesignIcons.PageLast` (:mdicon:`page-last`)
  * :const:`tlp.TulipMaterialDesignIcons.PageLayoutBody` (:mdicon:`page-layout-body`)
  * :const:`tlp.TulipMaterialDesignIcons.PageLayoutFooter` (:mdicon:`page-layout-footer`)
  * :const:`tlp.TulipMaterialDesignIcons.PageLayoutHeader` (:mdicon:`page-layout-header`)
  * :const:`tlp.TulipMaterialDesignIcons.PageLayoutSidebarLeft` (:mdicon:`page-layout-sidebar-left`)
  * :const:`tlp.TulipMaterialDesignIcons.PageLayoutSidebarRight` (:mdicon:`page-layout-sidebar-right`)
  * :const:`tlp.TulipMaterialDesignIcons.Palette` (:mdicon:`palette`)
  * :const:`tlp.TulipMaterialDesignIcons.PaletteAdvanced` (:mdicon:`palette-advanced`)
  * :const:`tlp.TulipMaterialDesignIcons.Panda` (:mdicon:`panda`)
  * :const:`tlp.TulipMaterialDesignIcons.Pandora` (:mdicon:`pandora`)
  * :const:`tlp.TulipMaterialDesignIcons.Panorama` (:mdicon:`panorama`)
  * :const:`tlp.TulipMaterialDesignIcons.PanoramaFisheye` (:mdicon:`panorama-fisheye`)
  * :const:`tlp.TulipMaterialDesignIcons.PanoramaHorizontal` (:mdicon:`panorama-horizontal`)
  * :const:`tlp.TulipMaterialDesignIcons.PanoramaVertical` (:mdicon:`panorama-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.PanoramaWideAngle` (:mdicon:`panorama-wide-angle`)
  * :const:`tlp.TulipMaterialDesignIcons.PaperCutVertical` (:mdicon:`paper-cut-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.Paperclip` (:mdicon:`paperclip`)
  * :const:`tlp.TulipMaterialDesignIcons.Parking` (:mdicon:`parking`)
  * :const:`tlp.TulipMaterialDesignIcons.Pause` (:mdicon:`pause`)
  * :const:`tlp.TulipMaterialDesignIcons.PauseCircle` (:mdicon:`pause-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.PauseCircleOutline` (:mdicon:`pause-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.PauseOctagon` (:mdicon:`pause-octagon`)
  * :const:`tlp.TulipMaterialDesignIcons.PauseOctagonOutline` (:mdicon:`pause-octagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Paw` (:mdicon:`paw`)
  * :const:`tlp.TulipMaterialDesignIcons.PawOff` (:mdicon:`paw-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Pen` (:mdicon:`pen`)
  * :const:`tlp.TulipMaterialDesignIcons.Pencil` (:mdicon:`pencil`)
  * :const:`tlp.TulipMaterialDesignIcons.PencilBox` (:mdicon:`pencil-box`)
  * :const:`tlp.TulipMaterialDesignIcons.PencilBoxOutline` (:mdicon:`pencil-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.PencilCircle` (:mdicon:`pencil-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.PencilCircleOutline` (:mdicon:`pencil-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.PencilLock` (:mdicon:`pencil-lock`)
  * :const:`tlp.TulipMaterialDesignIcons.PencilOff` (:mdicon:`pencil-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Pentagon` (:mdicon:`pentagon`)
  * :const:`tlp.TulipMaterialDesignIcons.PentagonOutline` (:mdicon:`pentagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Percent` (:mdicon:`percent`)
  * :const:`tlp.TulipMaterialDesignIcons.Periscope` (:mdicon:`periscope`)
  * :const:`tlp.TulipMaterialDesignIcons.Pharmacy` (:mdicon:`pharmacy`)
  * :const:`tlp.TulipMaterialDesignIcons.Phone` (:mdicon:`phone`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneBluetooth` (:mdicon:`phone-bluetooth`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneClassic` (:mdicon:`phone-classic`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneForward` (:mdicon:`phone-forward`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneHangup` (:mdicon:`phone-hangup`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneInTalk` (:mdicon:`phone-in-talk`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneIncoming` (:mdicon:`phone-incoming`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneLocked` (:mdicon:`phone-locked`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneLog` (:mdicon:`phone-log`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneMinus` (:mdicon:`phone-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneMissed` (:mdicon:`phone-missed`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneOutgoing` (:mdicon:`phone-outgoing`)
  * :const:`tlp.TulipMaterialDesignIcons.PhonePaused` (:mdicon:`phone-paused`)
  * :const:`tlp.TulipMaterialDesignIcons.PhonePlus` (:mdicon:`phone-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneSettings` (:mdicon:`phone-settings`)
  * :const:`tlp.TulipMaterialDesignIcons.PhoneVoip` (:mdicon:`phone-voip`)
  * :const:`tlp.TulipMaterialDesignIcons.Pi` (:mdicon:`pi`)
  * :const:`tlp.TulipMaterialDesignIcons.PiBox` (:mdicon:`pi-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Piano` (:mdicon:`piano`)
  * :const:`tlp.TulipMaterialDesignIcons.Pig` (:mdicon:`pig`)
  * :const:`tlp.TulipMaterialDesignIcons.Pill` (:mdicon:`pill`)
  * :const:`tlp.TulipMaterialDesignIcons.Pillar` (:mdicon:`pillar`)
  * :const:`tlp.TulipMaterialDesignIcons.Pin` (:mdicon:`pin`)
  * :const:`tlp.TulipMaterialDesignIcons.PinOff` (:mdicon:`pin-off`)
  * :const:`tlp.TulipMaterialDesignIcons.PineTree` (:mdicon:`pine-tree`)
  * :const:`tlp.TulipMaterialDesignIcons.PineTreeBox` (:mdicon:`pine-tree-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Pinterest` (:mdicon:`pinterest`)
  * :const:`tlp.TulipMaterialDesignIcons.PinterestBox` (:mdicon:`pinterest-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Pistol` (:mdicon:`pistol`)
  * :const:`tlp.TulipMaterialDesignIcons.Pizza` (:mdicon:`pizza`)
  * :const:`tlp.TulipMaterialDesignIcons.PlaneShield` (:mdicon:`plane-shield`)
  * :const:`tlp.TulipMaterialDesignIcons.Play` (:mdicon:`play`)
  * :const:`tlp.TulipMaterialDesignIcons.PlayBoxOutline` (:mdicon:`play-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.PlayCircle` (:mdicon:`play-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.PlayCircleOutline` (:mdicon:`play-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.PlayPause` (:mdicon:`play-pause`)
  * :const:`tlp.TulipMaterialDesignIcons.PlayProtectedContent` (:mdicon:`play-protected-content`)
  * :const:`tlp.TulipMaterialDesignIcons.PlaylistCheck` (:mdicon:`playlist-check`)
  * :const:`tlp.TulipMaterialDesignIcons.PlaylistMinus` (:mdicon:`playlist-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.PlaylistPlay` (:mdicon:`playlist-play`)
  * :const:`tlp.TulipMaterialDesignIcons.PlaylistPlus` (:mdicon:`playlist-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.PlaylistRemove` (:mdicon:`playlist-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.Playstation` (:mdicon:`playstation`)
  * :const:`tlp.TulipMaterialDesignIcons.Plex` (:mdicon:`plex`)
  * :const:`tlp.TulipMaterialDesignIcons.Plus` (:mdicon:`plus`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusBox` (:mdicon:`plus-box`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusBoxOutline` (:mdicon:`plus-box-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusCircle` (:mdicon:`plus-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusCircleMultipleOutline` (:mdicon:`plus-circle-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusCircleOutline` (:mdicon:`plus-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusNetwork` (:mdicon:`plus-network`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusOne` (:mdicon:`plus-one`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusOutline` (:mdicon:`plus-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Pocket` (:mdicon:`pocket`)
  * :const:`tlp.TulipMaterialDesignIcons.Pokeball` (:mdicon:`pokeball`)
  * :const:`tlp.TulipMaterialDesignIcons.Polaroid` (:mdicon:`polaroid`)
  * :const:`tlp.TulipMaterialDesignIcons.Poll` (:mdicon:`poll`)
  * :const:`tlp.TulipMaterialDesignIcons.PollBox` (:mdicon:`poll-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Polymer` (:mdicon:`polymer`)
  * :const:`tlp.TulipMaterialDesignIcons.Pool` (:mdicon:`pool`)
  * :const:`tlp.TulipMaterialDesignIcons.Popcorn` (:mdicon:`popcorn`)
  * :const:`tlp.TulipMaterialDesignIcons.Pot` (:mdicon:`pot`)
  * :const:`tlp.TulipMaterialDesignIcons.PotMix` (:mdicon:`pot-mix`)
  * :const:`tlp.TulipMaterialDesignIcons.Pound` (:mdicon:`pound`)
  * :const:`tlp.TulipMaterialDesignIcons.PoundBox` (:mdicon:`pound-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Power` (:mdicon:`power`)
  * :const:`tlp.TulipMaterialDesignIcons.PowerPlug` (:mdicon:`power-plug`)
  * :const:`tlp.TulipMaterialDesignIcons.PowerPlugOff` (:mdicon:`power-plug-off`)
  * :const:`tlp.TulipMaterialDesignIcons.PowerSettings` (:mdicon:`power-settings`)
  * :const:`tlp.TulipMaterialDesignIcons.PowerSocket` (:mdicon:`power-socket`)
  * :const:`tlp.TulipMaterialDesignIcons.Prescription` (:mdicon:`prescription`)
  * :const:`tlp.TulipMaterialDesignIcons.Presentation` (:mdicon:`presentation`)
  * :const:`tlp.TulipMaterialDesignIcons.PresentationPlay` (:mdicon:`presentation-play`)
  * :const:`tlp.TulipMaterialDesignIcons.Printer` (:mdicon:`printer`)
  * :const:`tlp.TulipMaterialDesignIcons.Printer3d` (:mdicon:`printer-3d`)
  * :const:`tlp.TulipMaterialDesignIcons.PrinterAlert` (:mdicon:`printer-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.PrinterSettings` (:mdicon:`printer-settings`)
  * :const:`tlp.TulipMaterialDesignIcons.PriorityHigh` (:mdicon:`priority-high`)
  * :const:`tlp.TulipMaterialDesignIcons.PriorityLow` (:mdicon:`priority-low`)
  * :const:`tlp.TulipMaterialDesignIcons.ProfessionalHexagon` (:mdicon:`professional-hexagon`)
  * :const:`tlp.TulipMaterialDesignIcons.Projector` (:mdicon:`projector`)
  * :const:`tlp.TulipMaterialDesignIcons.ProjectorScreen` (:mdicon:`projector-screen`)
  * :const:`tlp.TulipMaterialDesignIcons.Publish` (:mdicon:`publish`)
  * :const:`tlp.TulipMaterialDesignIcons.Pulse` (:mdicon:`pulse`)
  * :const:`tlp.TulipMaterialDesignIcons.Puzzle` (:mdicon:`puzzle`)
  * :const:`tlp.TulipMaterialDesignIcons.Qqchat` (:mdicon:`qqchat`)
  * :const:`tlp.TulipMaterialDesignIcons.Qrcode` (:mdicon:`qrcode`)
  * :const:`tlp.TulipMaterialDesignIcons.QrcodeScan` (:mdicon:`qrcode-scan`)
  * :const:`tlp.TulipMaterialDesignIcons.Quadcopter` (:mdicon:`quadcopter`)
  * :const:`tlp.TulipMaterialDesignIcons.QualityHigh` (:mdicon:`quality-high`)
  * :const:`tlp.TulipMaterialDesignIcons.Quicktime` (:mdicon:`quicktime`)
  * :const:`tlp.TulipMaterialDesignIcons.Radar` (:mdicon:`radar`)
  * :const:`tlp.TulipMaterialDesignIcons.Radiator` (:mdicon:`radiator`)
  * :const:`tlp.TulipMaterialDesignIcons.Radio` (:mdicon:`radio`)
  * :const:`tlp.TulipMaterialDesignIcons.RadioHandheld` (:mdicon:`radio-handheld`)
  * :const:`tlp.TulipMaterialDesignIcons.RadioTower` (:mdicon:`radio-tower`)
  * :const:`tlp.TulipMaterialDesignIcons.Radioactive` (:mdicon:`radioactive`)
  * :const:`tlp.TulipMaterialDesignIcons.RadioboxBlank` (:mdicon:`radiobox-blank`)
  * :const:`tlp.TulipMaterialDesignIcons.RadioboxMarked` (:mdicon:`radiobox-marked`)
  * :const:`tlp.TulipMaterialDesignIcons.Raspberrypi` (:mdicon:`raspberrypi`)
  * :const:`tlp.TulipMaterialDesignIcons.RayEnd` (:mdicon:`ray-end`)
  * :const:`tlp.TulipMaterialDesignIcons.RayEndArrow` (:mdicon:`ray-end-arrow`)
  * :const:`tlp.TulipMaterialDesignIcons.RayStart` (:mdicon:`ray-start`)
  * :const:`tlp.TulipMaterialDesignIcons.RayStartArrow` (:mdicon:`ray-start-arrow`)
  * :const:`tlp.TulipMaterialDesignIcons.RayStartEnd` (:mdicon:`ray-start-end`)
  * :const:`tlp.TulipMaterialDesignIcons.RayVertex` (:mdicon:`ray-vertex`)
  * :const:`tlp.TulipMaterialDesignIcons.Rdio` (:mdicon:`rdio`)
  * :const:`tlp.TulipMaterialDesignIcons.React` (:mdicon:`react`)
  * :const:`tlp.TulipMaterialDesignIcons.Read` (:mdicon:`read`)
  * :const:`tlp.TulipMaterialDesignIcons.Readability` (:mdicon:`readability`)
  * :const:`tlp.TulipMaterialDesignIcons.Receipt` (:mdicon:`receipt`)
  * :const:`tlp.TulipMaterialDesignIcons.Record` (:mdicon:`record`)
  * :const:`tlp.TulipMaterialDesignIcons.RecordRec` (:mdicon:`record-rec`)
  * :const:`tlp.TulipMaterialDesignIcons.Recycle` (:mdicon:`recycle`)
  * :const:`tlp.TulipMaterialDesignIcons.Reddit` (:mdicon:`reddit`)
  * :const:`tlp.TulipMaterialDesignIcons.Redo` (:mdicon:`redo`)
  * :const:`tlp.TulipMaterialDesignIcons.RedoVariant` (:mdicon:`redo-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Refresh` (:mdicon:`refresh`)
  * :const:`tlp.TulipMaterialDesignIcons.Regex` (:mdicon:`regex`)
  * :const:`tlp.TulipMaterialDesignIcons.RelativeScale` (:mdicon:`relative-scale`)
  * :const:`tlp.TulipMaterialDesignIcons.Reload` (:mdicon:`reload`)
  * :const:`tlp.TulipMaterialDesignIcons.Remote` (:mdicon:`remote`)
  * :const:`tlp.TulipMaterialDesignIcons.RenameBox` (:mdicon:`rename-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ReorderHorizontal` (:mdicon:`reorder-horizontal`)
  * :const:`tlp.TulipMaterialDesignIcons.ReorderVertical` (:mdicon:`reorder-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.Repeat` (:mdicon:`repeat`)
  * :const:`tlp.TulipMaterialDesignIcons.RepeatOff` (:mdicon:`repeat-off`)
  * :const:`tlp.TulipMaterialDesignIcons.RepeatOnce` (:mdicon:`repeat-once`)
  * :const:`tlp.TulipMaterialDesignIcons.Replay` (:mdicon:`replay`)
  * :const:`tlp.TulipMaterialDesignIcons.Reply` (:mdicon:`reply`)
  * :const:`tlp.TulipMaterialDesignIcons.ReplyAll` (:mdicon:`reply-all`)
  * :const:`tlp.TulipMaterialDesignIcons.Reproduction` (:mdicon:`reproduction`)
  * :const:`tlp.TulipMaterialDesignIcons.ResizeBottomRight` (:mdicon:`resize-bottom-right`)
  * :const:`tlp.TulipMaterialDesignIcons.Responsive` (:mdicon:`responsive`)
  * :const:`tlp.TulipMaterialDesignIcons.Restart` (:mdicon:`restart`)
  * :const:`tlp.TulipMaterialDesignIcons.Restore` (:mdicon:`restore`)
  * :const:`tlp.TulipMaterialDesignIcons.Rewind` (:mdicon:`rewind`)
  * :const:`tlp.TulipMaterialDesignIcons.RewindOutline` (:mdicon:`rewind-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Rhombus` (:mdicon:`rhombus`)
  * :const:`tlp.TulipMaterialDesignIcons.RhombusOutline` (:mdicon:`rhombus-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Ribbon` (:mdicon:`ribbon`)
  * :const:`tlp.TulipMaterialDesignIcons.Road` (:mdicon:`road`)
  * :const:`tlp.TulipMaterialDesignIcons.RoadVariant` (:mdicon:`road-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Robot` (:mdicon:`robot`)
  * :const:`tlp.TulipMaterialDesignIcons.Rocket` (:mdicon:`rocket`)
  * :const:`tlp.TulipMaterialDesignIcons.Roomba` (:mdicon:`roomba`)
  * :const:`tlp.TulipMaterialDesignIcons.Rotate3d` (:mdicon:`rotate-3d`)
  * :const:`tlp.TulipMaterialDesignIcons.RotateLeft` (:mdicon:`rotate-left`)
  * :const:`tlp.TulipMaterialDesignIcons.RotateLeftVariant` (:mdicon:`rotate-left-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.RotateRight` (:mdicon:`rotate-right`)
  * :const:`tlp.TulipMaterialDesignIcons.RotateRightVariant` (:mdicon:`rotate-right-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.RoundedCorner` (:mdicon:`rounded-corner`)
  * :const:`tlp.TulipMaterialDesignIcons.RouterWireless` (:mdicon:`router-wireless`)
  * :const:`tlp.TulipMaterialDesignIcons.Routes` (:mdicon:`routes`)
  * :const:`tlp.TulipMaterialDesignIcons.Rowing` (:mdicon:`rowing`)
  * :const:`tlp.TulipMaterialDesignIcons.Rss` (:mdicon:`rss`)
  * :const:`tlp.TulipMaterialDesignIcons.RssBox` (:mdicon:`rss-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Ruler` (:mdicon:`ruler`)
  * :const:`tlp.TulipMaterialDesignIcons.Run` (:mdicon:`run`)
  * :const:`tlp.TulipMaterialDesignIcons.RunFast` (:mdicon:`run-fast`)
  * :const:`tlp.TulipMaterialDesignIcons.Sale` (:mdicon:`sale`)
  * :const:`tlp.TulipMaterialDesignIcons.Satellite` (:mdicon:`satellite`)
  * :const:`tlp.TulipMaterialDesignIcons.SatelliteVariant` (:mdicon:`satellite-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Saxophone` (:mdicon:`saxophone`)
  * :const:`tlp.TulipMaterialDesignIcons.Scale` (:mdicon:`scale`)
  * :const:`tlp.TulipMaterialDesignIcons.ScaleBalance` (:mdicon:`scale-balance`)
  * :const:`tlp.TulipMaterialDesignIcons.ScaleBathroom` (:mdicon:`scale-bathroom`)
  * :const:`tlp.TulipMaterialDesignIcons.Scanner` (:mdicon:`scanner`)
  * :const:`tlp.TulipMaterialDesignIcons.School` (:mdicon:`school`)
  * :const:`tlp.TulipMaterialDesignIcons.ScreenRotation` (:mdicon:`screen-rotation`)
  * :const:`tlp.TulipMaterialDesignIcons.ScreenRotationLock` (:mdicon:`screen-rotation-lock`)
  * :const:`tlp.TulipMaterialDesignIcons.Screwdriver` (:mdicon:`screwdriver`)
  * :const:`tlp.TulipMaterialDesignIcons.Script` (:mdicon:`script`)
  * :const:`tlp.TulipMaterialDesignIcons.Sd` (:mdicon:`sd`)
  * :const:`tlp.TulipMaterialDesignIcons.Seal` (:mdicon:`seal`)
  * :const:`tlp.TulipMaterialDesignIcons.SearchWeb` (:mdicon:`search-web`)
  * :const:`tlp.TulipMaterialDesignIcons.SeatFlat` (:mdicon:`seat-flat`)
  * :const:`tlp.TulipMaterialDesignIcons.SeatFlatAngled` (:mdicon:`seat-flat-angled`)
  * :const:`tlp.TulipMaterialDesignIcons.SeatIndividualSuite` (:mdicon:`seat-individual-suite`)
  * :const:`tlp.TulipMaterialDesignIcons.SeatLegroomExtra` (:mdicon:`seat-legroom-extra`)
  * :const:`tlp.TulipMaterialDesignIcons.SeatLegroomNormal` (:mdicon:`seat-legroom-normal`)
  * :const:`tlp.TulipMaterialDesignIcons.SeatLegroomReduced` (:mdicon:`seat-legroom-reduced`)
  * :const:`tlp.TulipMaterialDesignIcons.SeatReclineExtra` (:mdicon:`seat-recline-extra`)
  * :const:`tlp.TulipMaterialDesignIcons.SeatReclineNormal` (:mdicon:`seat-recline-normal`)
  * :const:`tlp.TulipMaterialDesignIcons.Security` (:mdicon:`security`)
  * :const:`tlp.TulipMaterialDesignIcons.SecurityHome` (:mdicon:`security-home`)
  * :const:`tlp.TulipMaterialDesignIcons.SecurityNetwork` (:mdicon:`security-network`)
  * :const:`tlp.TulipMaterialDesignIcons.Select` (:mdicon:`select`)
  * :const:`tlp.TulipMaterialDesignIcons.SelectAll` (:mdicon:`select-all`)
  * :const:`tlp.TulipMaterialDesignIcons.SelectInverse` (:mdicon:`select-inverse`)
  * :const:`tlp.TulipMaterialDesignIcons.SelectOff` (:mdicon:`select-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Selection` (:mdicon:`selection`)
  * :const:`tlp.TulipMaterialDesignIcons.SelectionOff` (:mdicon:`selection-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Send` (:mdicon:`send`)
  * :const:`tlp.TulipMaterialDesignIcons.SerialPort` (:mdicon:`serial-port`)
  * :const:`tlp.TulipMaterialDesignIcons.Server` (:mdicon:`server`)
  * :const:`tlp.TulipMaterialDesignIcons.ServerMinus` (:mdicon:`server-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.ServerNetwork` (:mdicon:`server-network`)
  * :const:`tlp.TulipMaterialDesignIcons.ServerNetworkOff` (:mdicon:`server-network-off`)
  * :const:`tlp.TulipMaterialDesignIcons.ServerOff` (:mdicon:`server-off`)
  * :const:`tlp.TulipMaterialDesignIcons.ServerPlus` (:mdicon:`server-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.ServerRemove` (:mdicon:`server-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.ServerSecurity` (:mdicon:`server-security`)
  * :const:`tlp.TulipMaterialDesignIcons.SetAll` (:mdicon:`set-all`)
  * :const:`tlp.TulipMaterialDesignIcons.SetCenter` (:mdicon:`set-center`)
  * :const:`tlp.TulipMaterialDesignIcons.SetCenterRight` (:mdicon:`set-center-right`)
  * :const:`tlp.TulipMaterialDesignIcons.SetLeft` (:mdicon:`set-left`)
  * :const:`tlp.TulipMaterialDesignIcons.SetLeftCenter` (:mdicon:`set-left-center`)
  * :const:`tlp.TulipMaterialDesignIcons.SetLeftRight` (:mdicon:`set-left-right`)
  * :const:`tlp.TulipMaterialDesignIcons.SetNone` (:mdicon:`set-none`)
  * :const:`tlp.TulipMaterialDesignIcons.SetRight` (:mdicon:`set-right`)
  * :const:`tlp.TulipMaterialDesignIcons.Settings` (:mdicon:`settings`)
  * :const:`tlp.TulipMaterialDesignIcons.SettingsBox` (:mdicon:`settings-box`)
  * :const:`tlp.TulipMaterialDesignIcons.ShapeCirclePlus` (:mdicon:`shape-circle-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.ShapePlus` (:mdicon:`shape-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.ShapePolygonPlus` (:mdicon:`shape-polygon-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.ShapeRectanglePlus` (:mdicon:`shape-rectangle-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.ShapeSquarePlus` (:mdicon:`shape-square-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.Share` (:mdicon:`share`)
  * :const:`tlp.TulipMaterialDesignIcons.ShareVariant` (:mdicon:`share-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Shield` (:mdicon:`shield`)
  * :const:`tlp.TulipMaterialDesignIcons.ShieldHalfFull` (:mdicon:`shield-half-full`)
  * :const:`tlp.TulipMaterialDesignIcons.ShieldOutline` (:mdicon:`shield-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Shopping` (:mdicon:`shopping`)
  * :const:`tlp.TulipMaterialDesignIcons.ShoppingMusic` (:mdicon:`shopping-music`)
  * :const:`tlp.TulipMaterialDesignIcons.Shovel` (:mdicon:`shovel`)
  * :const:`tlp.TulipMaterialDesignIcons.ShovelOff` (:mdicon:`shovel-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Shredder` (:mdicon:`shredder`)
  * :const:`tlp.TulipMaterialDesignIcons.Shuffle` (:mdicon:`shuffle`)
  * :const:`tlp.TulipMaterialDesignIcons.ShuffleDisabled` (:mdicon:`shuffle-disabled`)
  * :const:`tlp.TulipMaterialDesignIcons.ShuffleVariant` (:mdicon:`shuffle-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Sigma` (:mdicon:`sigma`)
  * :const:`tlp.TulipMaterialDesignIcons.SigmaLower` (:mdicon:`sigma-lower`)
  * :const:`tlp.TulipMaterialDesignIcons.SignCaution` (:mdicon:`sign-caution`)
  * :const:`tlp.TulipMaterialDesignIcons.SignDirection` (:mdicon:`sign-direction`)
  * :const:`tlp.TulipMaterialDesignIcons.SignText` (:mdicon:`sign-text`)
  * :const:`tlp.TulipMaterialDesignIcons.Signal` (:mdicon:`signal`)
  * :const:`tlp.TulipMaterialDesignIcons.Signal2g` (:mdicon:`signal-2g`)
  * :const:`tlp.TulipMaterialDesignIcons.Signal3g` (:mdicon:`signal-3g`)
  * :const:`tlp.TulipMaterialDesignIcons.Signal4g` (:mdicon:`signal-4g`)
  * :const:`tlp.TulipMaterialDesignIcons.SignalHspa` (:mdicon:`signal-hspa`)
  * :const:`tlp.TulipMaterialDesignIcons.SignalHspaPlus` (:mdicon:`signal-hspa-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.SignalOff` (:mdicon:`signal-off`)
  * :const:`tlp.TulipMaterialDesignIcons.SignalVariant` (:mdicon:`signal-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Silverware` (:mdicon:`silverware`)
  * :const:`tlp.TulipMaterialDesignIcons.SilverwareFork` (:mdicon:`silverware-fork`)
  * :const:`tlp.TulipMaterialDesignIcons.SilverwareSpoon` (:mdicon:`silverware-spoon`)
  * :const:`tlp.TulipMaterialDesignIcons.SilverwareVariant` (:mdicon:`silverware-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Sim` (:mdicon:`sim`)
  * :const:`tlp.TulipMaterialDesignIcons.SimAlert` (:mdicon:`sim-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.SimOff` (:mdicon:`sim-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Sitemap` (:mdicon:`sitemap`)
  * :const:`tlp.TulipMaterialDesignIcons.SkipBackward` (:mdicon:`skip-backward`)
  * :const:`tlp.TulipMaterialDesignIcons.SkipForward` (:mdicon:`skip-forward`)
  * :const:`tlp.TulipMaterialDesignIcons.SkipNext` (:mdicon:`skip-next`)
  * :const:`tlp.TulipMaterialDesignIcons.SkipNextCircle` (:mdicon:`skip-next-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.SkipNextCircleOutline` (:mdicon:`skip-next-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.SkipPrevious` (:mdicon:`skip-previous`)
  * :const:`tlp.TulipMaterialDesignIcons.SkipPreviousCircle` (:mdicon:`skip-previous-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.SkipPreviousCircleOutline` (:mdicon:`skip-previous-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Skull` (:mdicon:`skull`)
  * :const:`tlp.TulipMaterialDesignIcons.Skype` (:mdicon:`skype`)
  * :const:`tlp.TulipMaterialDesignIcons.SkypeBusiness` (:mdicon:`skype-business`)
  * :const:`tlp.TulipMaterialDesignIcons.Slack` (:mdicon:`slack`)
  * :const:`tlp.TulipMaterialDesignIcons.Sleep` (:mdicon:`sleep`)
  * :const:`tlp.TulipMaterialDesignIcons.SleepOff` (:mdicon:`sleep-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Smoking` (:mdicon:`smoking`)
  * :const:`tlp.TulipMaterialDesignIcons.SmokingOff` (:mdicon:`smoking-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Snapchat` (:mdicon:`snapchat`)
  * :const:`tlp.TulipMaterialDesignIcons.Snowflake` (:mdicon:`snowflake`)
  * :const:`tlp.TulipMaterialDesignIcons.Snowman` (:mdicon:`snowman`)
  * :const:`tlp.TulipMaterialDesignIcons.Soccer` (:mdicon:`soccer`)
  * :const:`tlp.TulipMaterialDesignIcons.Sofa` (:mdicon:`sofa`)
  * :const:`tlp.TulipMaterialDesignIcons.Solid` (:mdicon:`solid`)
  * :const:`tlp.TulipMaterialDesignIcons.Sort` (:mdicon:`sort`)
  * :const:`tlp.TulipMaterialDesignIcons.SortAlphabetical` (:mdicon:`sort-alphabetical`)
  * :const:`tlp.TulipMaterialDesignIcons.SortAscending` (:mdicon:`sort-ascending`)
  * :const:`tlp.TulipMaterialDesignIcons.SortDescending` (:mdicon:`sort-descending`)
  * :const:`tlp.TulipMaterialDesignIcons.SortNumeric` (:mdicon:`sort-numeric`)
  * :const:`tlp.TulipMaterialDesignIcons.SortVariant` (:mdicon:`sort-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Soundcloud` (:mdicon:`soundcloud`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceBranch` (:mdicon:`source-branch`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceCommit` (:mdicon:`source-commit`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceCommitEnd` (:mdicon:`source-commit-end`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceCommitEndLocal` (:mdicon:`source-commit-end-local`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceCommitLocal` (:mdicon:`source-commit-local`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceCommitNextLocal` (:mdicon:`source-commit-next-local`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceCommitStart` (:mdicon:`source-commit-start`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceCommitStartNextLocal` (:mdicon:`source-commit-start-next-local`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceFork` (:mdicon:`source-fork`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceMerge` (:mdicon:`source-merge`)
  * :const:`tlp.TulipMaterialDesignIcons.SourcePull` (:mdicon:`source-pull`)
  * :const:`tlp.TulipMaterialDesignIcons.Speaker` (:mdicon:`speaker`)
  * :const:`tlp.TulipMaterialDesignIcons.SpeakerOff` (:mdicon:`speaker-off`)
  * :const:`tlp.TulipMaterialDesignIcons.SpeakerWireless` (:mdicon:`speaker-wireless`)
  * :const:`tlp.TulipMaterialDesignIcons.Speedometer` (:mdicon:`speedometer`)
  * :const:`tlp.TulipMaterialDesignIcons.Spellcheck` (:mdicon:`spellcheck`)
  * :const:`tlp.TulipMaterialDesignIcons.Spotify` (:mdicon:`spotify`)
  * :const:`tlp.TulipMaterialDesignIcons.Spotlight` (:mdicon:`spotlight`)
  * :const:`tlp.TulipMaterialDesignIcons.SpotlightBeam` (:mdicon:`spotlight-beam`)
  * :const:`tlp.TulipMaterialDesignIcons.Spray` (:mdicon:`spray`)
  * :const:`tlp.TulipMaterialDesignIcons.Square` (:mdicon:`square`)
  * :const:`tlp.TulipMaterialDesignIcons.SquareInc` (:mdicon:`square-inc`)
  * :const:`tlp.TulipMaterialDesignIcons.SquareIncCash` (:mdicon:`square-inc-cash`)
  * :const:`tlp.TulipMaterialDesignIcons.SquareOutline` (:mdicon:`square-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.SquareRoot` (:mdicon:`square-root`)
  * :const:`tlp.TulipMaterialDesignIcons.Stackexchange` (:mdicon:`stackexchange`)
  * :const:`tlp.TulipMaterialDesignIcons.Stackoverflow` (:mdicon:`stackoverflow`)
  * :const:`tlp.TulipMaterialDesignIcons.Stadium` (:mdicon:`stadium`)
  * :const:`tlp.TulipMaterialDesignIcons.Stairs` (:mdicon:`stairs`)
  * :const:`tlp.TulipMaterialDesignIcons.Star` (:mdicon:`star`)
  * :const:`tlp.TulipMaterialDesignIcons.StarCircle` (:mdicon:`star-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.StarHalf` (:mdicon:`star-half`)
  * :const:`tlp.TulipMaterialDesignIcons.StarOff` (:mdicon:`star-off`)
  * :const:`tlp.TulipMaterialDesignIcons.StarOutline` (:mdicon:`star-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Steam` (:mdicon:`steam`)
  * :const:`tlp.TulipMaterialDesignIcons.Steering` (:mdicon:`steering`)
  * :const:`tlp.TulipMaterialDesignIcons.StepBackward` (:mdicon:`step-backward`)
  * :const:`tlp.TulipMaterialDesignIcons.StepBackward2` (:mdicon:`step-backward-2`)
  * :const:`tlp.TulipMaterialDesignIcons.StepForward` (:mdicon:`step-forward`)
  * :const:`tlp.TulipMaterialDesignIcons.StepForward2` (:mdicon:`step-forward-2`)
  * :const:`tlp.TulipMaterialDesignIcons.Stethoscope` (:mdicon:`stethoscope`)
  * :const:`tlp.TulipMaterialDesignIcons.Sticker` (:mdicon:`sticker`)
  * :const:`tlp.TulipMaterialDesignIcons.StickerEmoji` (:mdicon:`sticker-emoji`)
  * :const:`tlp.TulipMaterialDesignIcons.Stocking` (:mdicon:`stocking`)
  * :const:`tlp.TulipMaterialDesignIcons.Stop` (:mdicon:`stop`)
  * :const:`tlp.TulipMaterialDesignIcons.StopCircle` (:mdicon:`stop-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.StopCircleOutline` (:mdicon:`stop-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Store` (:mdicon:`store`)
  * :const:`tlp.TulipMaterialDesignIcons.Store24Hour` (:mdicon:`store-24-hour`)
  * :const:`tlp.TulipMaterialDesignIcons.Stove` (:mdicon:`stove`)
  * :const:`tlp.TulipMaterialDesignIcons.SubdirectoryArrowLeft` (:mdicon:`subdirectory-arrow-left`)
  * :const:`tlp.TulipMaterialDesignIcons.SubdirectoryArrowRight` (:mdicon:`subdirectory-arrow-right`)
  * :const:`tlp.TulipMaterialDesignIcons.Subway` (:mdicon:`subway`)
  * :const:`tlp.TulipMaterialDesignIcons.SubwayVariant` (:mdicon:`subway-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Summit` (:mdicon:`summit`)
  * :const:`tlp.TulipMaterialDesignIcons.Sunglasses` (:mdicon:`sunglasses`)
  * :const:`tlp.TulipMaterialDesignIcons.SurroundSound` (:mdicon:`surround-sound`)
  * :const:`tlp.TulipMaterialDesignIcons.Svg` (:mdicon:`svg`)
  * :const:`tlp.TulipMaterialDesignIcons.SwapHorizontal` (:mdicon:`swap-horizontal`)
  * :const:`tlp.TulipMaterialDesignIcons.SwapVertical` (:mdicon:`swap-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.Swim` (:mdicon:`swim`)
  * :const:`tlp.TulipMaterialDesignIcons.Switch` (:mdicon:`switch`)
  * :const:`tlp.TulipMaterialDesignIcons.Sword` (:mdicon:`sword`)
  * :const:`tlp.TulipMaterialDesignIcons.SwordCross` (:mdicon:`sword-cross`)
  * :const:`tlp.TulipMaterialDesignIcons.Sync` (:mdicon:`sync`)
  * :const:`tlp.TulipMaterialDesignIcons.SyncAlert` (:mdicon:`sync-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.SyncOff` (:mdicon:`sync-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Tab` (:mdicon:`tab`)
  * :const:`tlp.TulipMaterialDesignIcons.TabPlus` (:mdicon:`tab-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.TabUnselected` (:mdicon:`tab-unselected`)
  * :const:`tlp.TulipMaterialDesignIcons.Table` (:mdicon:`table`)
  * :const:`tlp.TulipMaterialDesignIcons.TableColumnPlusAfter` (:mdicon:`table-column-plus-after`)
  * :const:`tlp.TulipMaterialDesignIcons.TableColumnPlusBefore` (:mdicon:`table-column-plus-before`)
  * :const:`tlp.TulipMaterialDesignIcons.TableColumnRemove` (:mdicon:`table-column-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.TableColumnWidth` (:mdicon:`table-column-width`)
  * :const:`tlp.TulipMaterialDesignIcons.TableEdit` (:mdicon:`table-edit`)
  * :const:`tlp.TulipMaterialDesignIcons.TableLarge` (:mdicon:`table-large`)
  * :const:`tlp.TulipMaterialDesignIcons.TableRowHeight` (:mdicon:`table-row-height`)
  * :const:`tlp.TulipMaterialDesignIcons.TableRowPlusAfter` (:mdicon:`table-row-plus-after`)
  * :const:`tlp.TulipMaterialDesignIcons.TableRowPlusBefore` (:mdicon:`table-row-plus-before`)
  * :const:`tlp.TulipMaterialDesignIcons.TableRowRemove` (:mdicon:`table-row-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.Tablet` (:mdicon:`tablet`)
  * :const:`tlp.TulipMaterialDesignIcons.TabletAndroid` (:mdicon:`tablet-android`)
  * :const:`tlp.TulipMaterialDesignIcons.TabletIpad` (:mdicon:`tablet-ipad`)
  * :const:`tlp.TulipMaterialDesignIcons.Taco` (:mdicon:`taco`)
  * :const:`tlp.TulipMaterialDesignIcons.Tag` (:mdicon:`tag`)
  * :const:`tlp.TulipMaterialDesignIcons.TagFaces` (:mdicon:`tag-faces`)
  * :const:`tlp.TulipMaterialDesignIcons.TagHeart` (:mdicon:`tag-heart`)
  * :const:`tlp.TulipMaterialDesignIcons.TagMultiple` (:mdicon:`tag-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.TagOutline` (:mdicon:`tag-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.TagPlus` (:mdicon:`tag-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.TagRemove` (:mdicon:`tag-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.TagTextOutline` (:mdicon:`tag-text-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Target` (:mdicon:`target`)
  * :const:`tlp.TulipMaterialDesignIcons.Taxi` (:mdicon:`taxi`)
  * :const:`tlp.TulipMaterialDesignIcons.Teamviewer` (:mdicon:`teamviewer`)
  * :const:`tlp.TulipMaterialDesignIcons.Telegram` (:mdicon:`telegram`)
  * :const:`tlp.TulipMaterialDesignIcons.Television` (:mdicon:`television`)
  * :const:`tlp.TulipMaterialDesignIcons.TelevisionGuide` (:mdicon:`television-guide`)
  * :const:`tlp.TulipMaterialDesignIcons.TemperatureCelsius` (:mdicon:`temperature-celsius`)
  * :const:`tlp.TulipMaterialDesignIcons.TemperatureFahrenheit` (:mdicon:`temperature-fahrenheit`)
  * :const:`tlp.TulipMaterialDesignIcons.TemperatureKelvin` (:mdicon:`temperature-kelvin`)
  * :const:`tlp.TulipMaterialDesignIcons.Tennis` (:mdicon:`tennis`)
  * :const:`tlp.TulipMaterialDesignIcons.Tent` (:mdicon:`tent`)
  * :const:`tlp.TulipMaterialDesignIcons.Terrain` (:mdicon:`terrain`)
  * :const:`tlp.TulipMaterialDesignIcons.TestTube` (:mdicon:`test-tube`)
  * :const:`tlp.TulipMaterialDesignIcons.TextShadow` (:mdicon:`text-shadow`)
  * :const:`tlp.TulipMaterialDesignIcons.TextToSpeech` (:mdicon:`text-to-speech`)
  * :const:`tlp.TulipMaterialDesignIcons.TextToSpeechOff` (:mdicon:`text-to-speech-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Textbox` (:mdicon:`textbox`)
  * :const:`tlp.TulipMaterialDesignIcons.Texture` (:mdicon:`texture`)
  * :const:`tlp.TulipMaterialDesignIcons.Theater` (:mdicon:`theater`)
  * :const:`tlp.TulipMaterialDesignIcons.ThemeLightDark` (:mdicon:`theme-light-dark`)
  * :const:`tlp.TulipMaterialDesignIcons.Thermometer` (:mdicon:`thermometer`)
  * :const:`tlp.TulipMaterialDesignIcons.ThermometerLines` (:mdicon:`thermometer-lines`)
  * :const:`tlp.TulipMaterialDesignIcons.ThumbDown` (:mdicon:`thumb-down`)
  * :const:`tlp.TulipMaterialDesignIcons.ThumbDownOutline` (:mdicon:`thumb-down-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ThumbUp` (:mdicon:`thumb-up`)
  * :const:`tlp.TulipMaterialDesignIcons.ThumbUpOutline` (:mdicon:`thumb-up-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ThumbsUpDown` (:mdicon:`thumbs-up-down`)
  * :const:`tlp.TulipMaterialDesignIcons.Ticket` (:mdicon:`ticket`)
  * :const:`tlp.TulipMaterialDesignIcons.TicketAccount` (:mdicon:`ticket-account`)
  * :const:`tlp.TulipMaterialDesignIcons.TicketConfirmation` (:mdicon:`ticket-confirmation`)
  * :const:`tlp.TulipMaterialDesignIcons.TicketPercent` (:mdicon:`ticket-percent`)
  * :const:`tlp.TulipMaterialDesignIcons.Tie` (:mdicon:`tie`)
  * :const:`tlp.TulipMaterialDesignIcons.Tilde` (:mdicon:`tilde`)
  * :const:`tlp.TulipMaterialDesignIcons.Timelapse` (:mdicon:`timelapse`)
  * :const:`tlp.TulipMaterialDesignIcons.Timer` (:mdicon:`timer`)
  * :const:`tlp.TulipMaterialDesignIcons.Timer10` (:mdicon:`timer-10`)
  * :const:`tlp.TulipMaterialDesignIcons.Timer3` (:mdicon:`timer-3`)
  * :const:`tlp.TulipMaterialDesignIcons.TimerOff` (:mdicon:`timer-off`)
  * :const:`tlp.TulipMaterialDesignIcons.TimerSand` (:mdicon:`timer-sand`)
  * :const:`tlp.TulipMaterialDesignIcons.TimerSandEmpty` (:mdicon:`timer-sand-empty`)
  * :const:`tlp.TulipMaterialDesignIcons.TimerSandFull` (:mdicon:`timer-sand-full`)
  * :const:`tlp.TulipMaterialDesignIcons.Timetable` (:mdicon:`timetable`)
  * :const:`tlp.TulipMaterialDesignIcons.ToggleSwitch` (:mdicon:`toggle-switch`)
  * :const:`tlp.TulipMaterialDesignIcons.ToggleSwitchOff` (:mdicon:`toggle-switch-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Tooltip` (:mdicon:`tooltip`)
  * :const:`tlp.TulipMaterialDesignIcons.TooltipEdit` (:mdicon:`tooltip-edit`)
  * :const:`tlp.TulipMaterialDesignIcons.TooltipImage` (:mdicon:`tooltip-image`)
  * :const:`tlp.TulipMaterialDesignIcons.TooltipOutline` (:mdicon:`tooltip-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.TooltipOutlinePlus` (:mdicon:`tooltip-outline-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.TooltipText` (:mdicon:`tooltip-text`)
  * :const:`tlp.TulipMaterialDesignIcons.Tooth` (:mdicon:`tooth`)
  * :const:`tlp.TulipMaterialDesignIcons.Tor` (:mdicon:`tor`)
  * :const:`tlp.TulipMaterialDesignIcons.TowerBeach` (:mdicon:`tower-beach`)
  * :const:`tlp.TulipMaterialDesignIcons.TowerFire` (:mdicon:`tower-fire`)
  * :const:`tlp.TulipMaterialDesignIcons.TrafficLight` (:mdicon:`traffic-light`)
  * :const:`tlp.TulipMaterialDesignIcons.Train` (:mdicon:`train`)
  * :const:`tlp.TulipMaterialDesignIcons.Tram` (:mdicon:`tram`)
  * :const:`tlp.TulipMaterialDesignIcons.Transcribe` (:mdicon:`transcribe`)
  * :const:`tlp.TulipMaterialDesignIcons.TranscribeClose` (:mdicon:`transcribe-close`)
  * :const:`tlp.TulipMaterialDesignIcons.Transfer` (:mdicon:`transfer`)
  * :const:`tlp.TulipMaterialDesignIcons.TransitTransfer` (:mdicon:`transit-transfer`)
  * :const:`tlp.TulipMaterialDesignIcons.Translate` (:mdicon:`translate`)
  * :const:`tlp.TulipMaterialDesignIcons.TreasureChest` (:mdicon:`treasure-chest`)
  * :const:`tlp.TulipMaterialDesignIcons.Tree` (:mdicon:`tree`)
  * :const:`tlp.TulipMaterialDesignIcons.Trello` (:mdicon:`trello`)
  * :const:`tlp.TulipMaterialDesignIcons.TrendingDown` (:mdicon:`trending-down`)
  * :const:`tlp.TulipMaterialDesignIcons.TrendingNeutral` (:mdicon:`trending-neutral`)
  * :const:`tlp.TulipMaterialDesignIcons.TrendingUp` (:mdicon:`trending-up`)
  * :const:`tlp.TulipMaterialDesignIcons.Triangle` (:mdicon:`triangle`)
  * :const:`tlp.TulipMaterialDesignIcons.TriangleOutline` (:mdicon:`triangle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Trophy` (:mdicon:`trophy`)
  * :const:`tlp.TulipMaterialDesignIcons.TrophyAward` (:mdicon:`trophy-award`)
  * :const:`tlp.TulipMaterialDesignIcons.TrophyOutline` (:mdicon:`trophy-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.TrophyVariant` (:mdicon:`trophy-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.TrophyVariantOutline` (:mdicon:`trophy-variant-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Truck` (:mdicon:`truck`)
  * :const:`tlp.TulipMaterialDesignIcons.TruckDelivery` (:mdicon:`truck-delivery`)
  * :const:`tlp.TulipMaterialDesignIcons.TruckFast` (:mdicon:`truck-fast`)
  * :const:`tlp.TulipMaterialDesignIcons.TruckTrailer` (:mdicon:`truck-trailer`)
  * :const:`tlp.TulipMaterialDesignIcons.TshirtCrew` (:mdicon:`tshirt-crew`)
  * :const:`tlp.TulipMaterialDesignIcons.TshirtV` (:mdicon:`tshirt-v`)
  * :const:`tlp.TulipMaterialDesignIcons.Tumblr` (:mdicon:`tumblr`)
  * :const:`tlp.TulipMaterialDesignIcons.TumblrReblog` (:mdicon:`tumblr-reblog`)
  * :const:`tlp.TulipMaterialDesignIcons.Tune` (:mdicon:`tune`)
  * :const:`tlp.TulipMaterialDesignIcons.TuneVertical` (:mdicon:`tune-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.Twitch` (:mdicon:`twitch`)
  * :const:`tlp.TulipMaterialDesignIcons.Twitter` (:mdicon:`twitter`)
  * :const:`tlp.TulipMaterialDesignIcons.TwitterBox` (:mdicon:`twitter-box`)
  * :const:`tlp.TulipMaterialDesignIcons.TwitterCircle` (:mdicon:`twitter-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.TwitterRetweet` (:mdicon:`twitter-retweet`)
  * :const:`tlp.TulipMaterialDesignIcons.Uber` (:mdicon:`uber`)
  * :const:`tlp.TulipMaterialDesignIcons.Ubuntu` (:mdicon:`ubuntu`)
  * :const:`tlp.TulipMaterialDesignIcons.Umbraco` (:mdicon:`umbraco`)
  * :const:`tlp.TulipMaterialDesignIcons.Umbrella` (:mdicon:`umbrella`)
  * :const:`tlp.TulipMaterialDesignIcons.UmbrellaOutline` (:mdicon:`umbrella-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Undo` (:mdicon:`undo`)
  * :const:`tlp.TulipMaterialDesignIcons.UndoVariant` (:mdicon:`undo-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.UnfoldLessHorizontal` (:mdicon:`unfold-less-horizontal`)
  * :const:`tlp.TulipMaterialDesignIcons.UnfoldLessVertical` (:mdicon:`unfold-less-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.UnfoldMoreHorizontal` (:mdicon:`unfold-more-horizontal`)
  * :const:`tlp.TulipMaterialDesignIcons.UnfoldMoreVertical` (:mdicon:`unfold-more-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.Ungroup` (:mdicon:`ungroup`)
  * :const:`tlp.TulipMaterialDesignIcons.Unity` (:mdicon:`unity`)
  * :const:`tlp.TulipMaterialDesignIcons.Untappd` (:mdicon:`untappd`)
  * :const:`tlp.TulipMaterialDesignIcons.Update` (:mdicon:`update`)
  * :const:`tlp.TulipMaterialDesignIcons.Upload` (:mdicon:`upload`)
  * :const:`tlp.TulipMaterialDesignIcons.UploadNetwork` (:mdicon:`upload-network`)
  * :const:`tlp.TulipMaterialDesignIcons.Usb` (:mdicon:`usb`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorArrangeAbove` (:mdicon:`vector-arrange-above`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorArrangeBelow` (:mdicon:`vector-arrange-below`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorCircle` (:mdicon:`vector-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorCircleVariant` (:mdicon:`vector-circle-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorCombine` (:mdicon:`vector-combine`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorCurve` (:mdicon:`vector-curve`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorDifference` (:mdicon:`vector-difference`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorDifferenceAb` (:mdicon:`vector-difference-ab`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorDifferenceBa` (:mdicon:`vector-difference-ba`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorIntersection` (:mdicon:`vector-intersection`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorLine` (:mdicon:`vector-line`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorPoint` (:mdicon:`vector-point`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorPolygon` (:mdicon:`vector-polygon`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorPolyline` (:mdicon:`vector-polyline`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorRadius` (:mdicon:`vector-radius`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorRectangle` (:mdicon:`vector-rectangle`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorSelection` (:mdicon:`vector-selection`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorSquare` (:mdicon:`vector-square`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorTriangle` (:mdicon:`vector-triangle`)
  * :const:`tlp.TulipMaterialDesignIcons.VectorUnion` (:mdicon:`vector-union`)
  * :const:`tlp.TulipMaterialDesignIcons.Verified` (:mdicon:`verified`)
  * :const:`tlp.TulipMaterialDesignIcons.Vibrate` (:mdicon:`vibrate`)
  * :const:`tlp.TulipMaterialDesignIcons.Video` (:mdicon:`video`)
  * :const:`tlp.TulipMaterialDesignIcons.VideoOff` (:mdicon:`video-off`)
  * :const:`tlp.TulipMaterialDesignIcons.VideoSwitch` (:mdicon:`video-switch`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewAgenda` (:mdicon:`view-agenda`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewArray` (:mdicon:`view-array`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewCarousel` (:mdicon:`view-carousel`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewColumn` (:mdicon:`view-column`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewDashboard` (:mdicon:`view-dashboard`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewDay` (:mdicon:`view-day`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewGrid` (:mdicon:`view-grid`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewHeadline` (:mdicon:`view-headline`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewList` (:mdicon:`view-list`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewModule` (:mdicon:`view-module`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewParallel` (:mdicon:`view-parallel`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewQuilt` (:mdicon:`view-quilt`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewSequential` (:mdicon:`view-sequential`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewStream` (:mdicon:`view-stream`)
  * :const:`tlp.TulipMaterialDesignIcons.ViewWeek` (:mdicon:`view-week`)
  * :const:`tlp.TulipMaterialDesignIcons.Vimeo` (:mdicon:`vimeo`)
  * :const:`tlp.TulipMaterialDesignIcons.Vine` (:mdicon:`vine`)
  * :const:`tlp.TulipMaterialDesignIcons.Violin` (:mdicon:`violin`)
  * :const:`tlp.TulipMaterialDesignIcons.Visualstudio` (:mdicon:`visualstudio`)
  * :const:`tlp.TulipMaterialDesignIcons.Vk` (:mdicon:`vk`)
  * :const:`tlp.TulipMaterialDesignIcons.VkBox` (:mdicon:`vk-box`)
  * :const:`tlp.TulipMaterialDesignIcons.VkCircle` (:mdicon:`vk-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.Vlc` (:mdicon:`vlc`)
  * :const:`tlp.TulipMaterialDesignIcons.Voice` (:mdicon:`voice`)
  * :const:`tlp.TulipMaterialDesignIcons.Voicemail` (:mdicon:`voicemail`)
  * :const:`tlp.TulipMaterialDesignIcons.VolumeHigh` (:mdicon:`volume-high`)
  * :const:`tlp.TulipMaterialDesignIcons.VolumeLow` (:mdicon:`volume-low`)
  * :const:`tlp.TulipMaterialDesignIcons.VolumeMedium` (:mdicon:`volume-medium`)
  * :const:`tlp.TulipMaterialDesignIcons.VolumeMinus` (:mdicon:`volume-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.VolumeMute` (:mdicon:`volume-mute`)
  * :const:`tlp.TulipMaterialDesignIcons.VolumeOff` (:mdicon:`volume-off`)
  * :const:`tlp.TulipMaterialDesignIcons.VolumePlus` (:mdicon:`volume-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.Vpn` (:mdicon:`vpn`)
  * :const:`tlp.TulipMaterialDesignIcons.Walk` (:mdicon:`walk`)
  * :const:`tlp.TulipMaterialDesignIcons.Wallet` (:mdicon:`wallet`)
  * :const:`tlp.TulipMaterialDesignIcons.WalletGiftcard` (:mdicon:`wallet-giftcard`)
  * :const:`tlp.TulipMaterialDesignIcons.WalletMembership` (:mdicon:`wallet-membership`)
  * :const:`tlp.TulipMaterialDesignIcons.WalletTravel` (:mdicon:`wallet-travel`)
  * :const:`tlp.TulipMaterialDesignIcons.Wan` (:mdicon:`wan`)
  * :const:`tlp.TulipMaterialDesignIcons.WashingMachine` (:mdicon:`washing-machine`)
  * :const:`tlp.TulipMaterialDesignIcons.Watch` (:mdicon:`watch`)
  * :const:`tlp.TulipMaterialDesignIcons.WatchExport` (:mdicon:`watch-export`)
  * :const:`tlp.TulipMaterialDesignIcons.WatchImport` (:mdicon:`watch-import`)
  * :const:`tlp.TulipMaterialDesignIcons.WatchVibrate` (:mdicon:`watch-vibrate`)
  * :const:`tlp.TulipMaterialDesignIcons.Water` (:mdicon:`water`)
  * :const:`tlp.TulipMaterialDesignIcons.WaterOff` (:mdicon:`water-off`)
  * :const:`tlp.TulipMaterialDesignIcons.WaterPercent` (:mdicon:`water-percent`)
  * :const:`tlp.TulipMaterialDesignIcons.WaterPump` (:mdicon:`water-pump`)
  * :const:`tlp.TulipMaterialDesignIcons.Watermark` (:mdicon:`watermark`)
  * :const:`tlp.TulipMaterialDesignIcons.Waves` (:mdicon:`waves`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherCloudy` (:mdicon:`weather-cloudy`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherFog` (:mdicon:`weather-fog`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherHail` (:mdicon:`weather-hail`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherLightning` (:mdicon:`weather-lightning`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherLightningRainy` (:mdicon:`weather-lightning-rainy`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherNight` (:mdicon:`weather-night`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherPartlycloudy` (:mdicon:`weather-partlycloudy`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherPouring` (:mdicon:`weather-pouring`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherRainy` (:mdicon:`weather-rainy`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherSnowy` (:mdicon:`weather-snowy`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherSnowyRainy` (:mdicon:`weather-snowy-rainy`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherSunny` (:mdicon:`weather-sunny`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherSunset` (:mdicon:`weather-sunset`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherSunsetDown` (:mdicon:`weather-sunset-down`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherSunsetUp` (:mdicon:`weather-sunset-up`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherWindy` (:mdicon:`weather-windy`)
  * :const:`tlp.TulipMaterialDesignIcons.WeatherWindyVariant` (:mdicon:`weather-windy-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Web` (:mdicon:`web`)
  * :const:`tlp.TulipMaterialDesignIcons.Webcam` (:mdicon:`webcam`)
  * :const:`tlp.TulipMaterialDesignIcons.Webhook` (:mdicon:`webhook`)
  * :const:`tlp.TulipMaterialDesignIcons.Webpack` (:mdicon:`webpack`)
  * :const:`tlp.TulipMaterialDesignIcons.Wechat` (:mdicon:`wechat`)
  * :const:`tlp.TulipMaterialDesignIcons.Weight` (:mdicon:`weight`)
  * :const:`tlp.TulipMaterialDesignIcons.WeightKilogram` (:mdicon:`weight-kilogram`)
  * :const:`tlp.TulipMaterialDesignIcons.Whatsapp` (:mdicon:`whatsapp`)
  * :const:`tlp.TulipMaterialDesignIcons.WheelchairAccessibility` (:mdicon:`wheelchair-accessibility`)
  * :const:`tlp.TulipMaterialDesignIcons.WhiteBalanceAuto` (:mdicon:`white-balance-auto`)
  * :const:`tlp.TulipMaterialDesignIcons.WhiteBalanceIncandescent` (:mdicon:`white-balance-incandescent`)
  * :const:`tlp.TulipMaterialDesignIcons.WhiteBalanceIridescent` (:mdicon:`white-balance-iridescent`)
  * :const:`tlp.TulipMaterialDesignIcons.WhiteBalanceSunny` (:mdicon:`white-balance-sunny`)
  * :const:`tlp.TulipMaterialDesignIcons.Widgets` (:mdicon:`widgets`)
  * :const:`tlp.TulipMaterialDesignIcons.Wifi` (:mdicon:`wifi`)
  * :const:`tlp.TulipMaterialDesignIcons.WifiOff` (:mdicon:`wifi-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Wii` (:mdicon:`wii`)
  * :const:`tlp.TulipMaterialDesignIcons.Wiiu` (:mdicon:`wiiu`)
  * :const:`tlp.TulipMaterialDesignIcons.Wikipedia` (:mdicon:`wikipedia`)
  * :const:`tlp.TulipMaterialDesignIcons.WindowClose` (:mdicon:`window-close`)
  * :const:`tlp.TulipMaterialDesignIcons.WindowClosed` (:mdicon:`window-closed`)
  * :const:`tlp.TulipMaterialDesignIcons.WindowMaximize` (:mdicon:`window-maximize`)
  * :const:`tlp.TulipMaterialDesignIcons.WindowMinimize` (:mdicon:`window-minimize`)
  * :const:`tlp.TulipMaterialDesignIcons.WindowOpen` (:mdicon:`window-open`)
  * :const:`tlp.TulipMaterialDesignIcons.WindowRestore` (:mdicon:`window-restore`)
  * :const:`tlp.TulipMaterialDesignIcons.Windows` (:mdicon:`windows`)
  * :const:`tlp.TulipMaterialDesignIcons.Wordpress` (:mdicon:`wordpress`)
  * :const:`tlp.TulipMaterialDesignIcons.Worker` (:mdicon:`worker`)
  * :const:`tlp.TulipMaterialDesignIcons.Wrap` (:mdicon:`wrap`)
  * :const:`tlp.TulipMaterialDesignIcons.Wrench` (:mdicon:`wrench`)
  * :const:`tlp.TulipMaterialDesignIcons.Wunderlist` (:mdicon:`wunderlist`)
  * :const:`tlp.TulipMaterialDesignIcons.Xaml` (:mdicon:`xaml`)
  * :const:`tlp.TulipMaterialDesignIcons.Xbox` (:mdicon:`xbox`)
  * :const:`tlp.TulipMaterialDesignIcons.XboxController` (:mdicon:`xbox-controller`)
  * :const:`tlp.TulipMaterialDesignIcons.XboxControllerBatteryAlert` (:mdicon:`xbox-controller-battery-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.XboxControllerBatteryEmpty` (:mdicon:`xbox-controller-battery-empty`)
  * :const:`tlp.TulipMaterialDesignIcons.XboxControllerBatteryFull` (:mdicon:`xbox-controller-battery-full`)
  * :const:`tlp.TulipMaterialDesignIcons.XboxControllerBatteryLow` (:mdicon:`xbox-controller-battery-low`)
  * :const:`tlp.TulipMaterialDesignIcons.XboxControllerBatteryMedium` (:mdicon:`xbox-controller-battery-medium`)
  * :const:`tlp.TulipMaterialDesignIcons.XboxControllerBatteryUnknown` (:mdicon:`xbox-controller-battery-unknown`)
  * :const:`tlp.TulipMaterialDesignIcons.XboxControllerOff` (:mdicon:`xbox-controller-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Xda` (:mdicon:`xda`)
  * :const:`tlp.TulipMaterialDesignIcons.Xing` (:mdicon:`xing`)
  * :const:`tlp.TulipMaterialDesignIcons.XingBox` (:mdicon:`xing-box`)
  * :const:`tlp.TulipMaterialDesignIcons.XingCircle` (:mdicon:`xing-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.Xml` (:mdicon:`xml`)
  * :const:`tlp.TulipMaterialDesignIcons.Yammer` (:mdicon:`yammer`)
  * :const:`tlp.TulipMaterialDesignIcons.Yeast` (:mdicon:`yeast`)
  * :const:`tlp.TulipMaterialDesignIcons.Yelp` (:mdicon:`yelp`)
  * :const:`tlp.TulipMaterialDesignIcons.YinYang` (:mdicon:`yin-yang`)
  * :const:`tlp.TulipMaterialDesignIcons.YoutubePlay` (:mdicon:`youtube-play`)
  * :const:`tlp.TulipMaterialDesignIcons.ZipBox` (:mdicon:`zip-box`)

%End
%End

public:

  static std::string getMaterialDesignIconsTrueTypeFileLocation();
%Docstring
tlp.TulipMaterialDesignIcons.getMaterialDesignIconsTrueTypeFileLocation()

Returns the location of the Material Design Icons TrueType font file bundled with Tulip.

:rtype: string
%End

  static const std::vector<std::string> &getSupportedMaterialDesignIcons();
%Docstring
tlp.TulipMaterialDesignIcons.getSupportedMaterialDesignIcons()

Returns the list of supported Material Design icons names.

:rtype: list of string
%End

  static bool isMaterialDesignIconSupported(const std::string &iconName);
%Docstring
tlp.TulipMaterialDesignIcons.isMaterialDesignIconSupported(iconName)

Checks if the provided Material Design icon name is supported.

:param iconName: the name of the icon to check support
:type iconName: string
:rtype: boolean
%End

  static unsigned int getMaterialDesignIconCodePoint(const std::string &iconName);
%Docstring
tlp.TulipMaterialDesignIcons.getMaterialDesignIconCodePoint(iconName)

Returns the Unicode code point associated to an icon name.

:param iconName: the name of the icon to get the codepoint
:type iconName: string
:rtype: integer
%End


  static std::string getMaterialDesignIconUtf8String(const std::string &iconName);
%Docstring
tlp.TulipMaterialDesignIcons.getMaterialDesignIconUtf8String(iconName)

Returns an UTF-8 encoded string of a Material Design icon.

:param iconName: a Material Design icon name
:type iconName: string
:rtype: string
%End

 static const std::string AccessPoint;
 static const std::string AccessPointNetwork;
 static const std::string Account;
 static const std::string AccountAlert;
 static const std::string AccountBox;
 static const std::string AccountBoxOutline;
 static const std::string AccountCardDetails;
 static const std::string AccountCheck;
 static const std::string AccountCircle;
 static const std::string AccountConvert;
 static const std::string AccountEdit;
 static const std::string AccountKey;
 static const std::string AccountLocation;
 static const std::string AccountMinus;
 static const std::string AccountMultiple;
 static const std::string AccountMultipleMinus;
 static const std::string AccountMultipleOutline;
 static const std::string AccountMultiplePlus;
 static const std::string AccountNetwork;
 static const std::string AccountOff;
 static const std::string AccountOutline;
 static const std::string AccountPlus;
 static const std::string AccountRemove;
 static const std::string AccountSearch;
 static const std::string AccountSettings;
 static const std::string AccountSettingsVariant;
 static const std::string AccountStar;
 static const std::string AccountSwitch;
 static const std::string Adjust;
 static const std::string AirConditioner;
 static const std::string Airballoon;
 static const std::string Airplane;
 static const std::string AirplaneLanding;
 static const std::string AirplaneOff;
 static const std::string AirplaneTakeoff;
 static const std::string Airplay;
 static const std::string Alarm;
 static const std::string AlarmCheck;
 static const std::string AlarmMultiple;
 static const std::string AlarmOff;
 static const std::string AlarmPlus;
 static const std::string AlarmSnooze;
 static const std::string Album;
 static const std::string Alert;
 static const std::string AlertBox;
 static const std::string AlertCircle;
 static const std::string AlertCircleOutline;
 static const std::string AlertDecagram;
 static const std::string AlertOctagon;
 static const std::string AlertOctagram;
 static const std::string AlertOutline;
 static const std::string AllInclusive;
 static const std::string Alpha;
 static const std::string Alphabetical;
 static const std::string Altimeter;
 static const std::string Amazon;
 static const std::string AmazonClouddrive;
 static const std::string Ambulance;
 static const std::string Amplifier;
 static const std::string Anchor;
 static const std::string Android;
 static const std::string AndroidDebugBridge;
 static const std::string AndroidStudio;
 static const std::string Angular;
 static const std::string Angularjs;
 static const std::string Animation;
 static const std::string Apple;
 static const std::string AppleFinder;
 static const std::string AppleIos;
 static const std::string AppleKeyboardCaps;
 static const std::string AppleKeyboardCommand;
 static const std::string AppleKeyboardControl;
 static const std::string AppleKeyboardOption;
 static const std::string AppleKeyboardShift;
 static const std::string AppleMobileme;
 static const std::string AppleSafari;
 static const std::string Application;
 static const std::string Apps;
 static const std::string Archive;
 static const std::string ArrangeBringForward;
 static const std::string ArrangeBringToFront;
 static const std::string ArrangeSendBackward;
 static const std::string ArrangeSendToBack;
 static const std::string ArrowAll;
 static const std::string ArrowBottomLeft;
 static const std::string ArrowBottomRight;
 static const std::string ArrowCompress;
 static const std::string ArrowCompressAll;
 static const std::string ArrowDown;
 static const std::string ArrowDownBold;
 static const std::string ArrowDownBoldBox;
 static const std::string ArrowDownBoldBoxOutline;
 static const std::string ArrowDownBoldCircle;
 static const std::string ArrowDownBoldCircleOutline;
 static const std::string ArrowDownBoldHexagonOutline;
 static const std::string ArrowDownBox;
 static const std::string ArrowDownDropCircle;
 static const std::string ArrowDownDropCircleOutline;
 static const std::string ArrowDownThick;
 static const std::string ArrowExpand;
 static const std::string ArrowExpandAll;
 static const std::string ArrowLeft;
 static const std::string ArrowLeftBold;
 static const std::string ArrowLeftBoldBox;
 static const std::string ArrowLeftBoldBoxOutline;
 static const std::string ArrowLeftBoldCircle;
 static const std::string ArrowLeftBoldCircleOutline;
 static const std::string ArrowLeftBoldHexagonOutline;
 static const std::string ArrowLeftBox;
 static const std::string ArrowLeftDropCircle;
 static const std::string ArrowLeftDropCircleOutline;
 static const std::string ArrowLeftThick;
 static const std::string ArrowRight;
 static const std::string ArrowRightBold;
 static const std::string ArrowRightBoldBox;
 static const std::string ArrowRightBoldBoxOutline;
 static const std::string ArrowRightBoldCircle;
 static const std::string ArrowRightBoldCircleOutline;
 static const std::string ArrowRightBoldHexagonOutline;
 static const std::string ArrowRightBox;
 static const std::string ArrowRightDropCircle;
 static const std::string ArrowRightDropCircleOutline;
 static const std::string ArrowRightThick;
 static const std::string ArrowTopLeft;
 static const std::string ArrowTopRight;
 static const std::string ArrowUp;
 static const std::string ArrowUpBold;
 static const std::string ArrowUpBoldBox;
 static const std::string ArrowUpBoldBoxOutline;
 static const std::string ArrowUpBoldCircle;
 static const std::string ArrowUpBoldCircleOutline;
 static const std::string ArrowUpBoldHexagonOutline;
 static const std::string ArrowUpBox;
 static const std::string ArrowUpDropCircle;
 static const std::string ArrowUpDropCircleOutline;
 static const std::string ArrowUpThick;
 static const std::string Assistant;
 static const std::string Asterisk;
 static const std::string At;
 static const std::string Atom;
 static const std::string Attachment;
 static const std::string Audiobook;
 static const std::string AutoFix;
 static const std::string AutoUpload;
 static const std::string Autorenew;
 static const std::string AvTimer;
 static const std::string Baby;
 static const std::string BabyBuggy;
 static const std::string Backburger;
 static const std::string Backspace;
 static const std::string BackupRestore;
 static const std::string Bandcamp;
 static const std::string Bank;
 static const std::string Barcode;
 static const std::string BarcodeScan;
 static const std::string Barley;
 static const std::string Barrel;
 static const std::string Basecamp;
 static const std::string Basket;
 static const std::string BasketFill;
 static const std::string BasketUnfill;
 static const std::string Battery;
 static const std::string Battery10;
 static const std::string Battery20;
 static const std::string Battery30;
 static const std::string Battery40;
 static const std::string Battery50;
 static const std::string Battery60;
 static const std::string Battery70;
 static const std::string Battery80;
 static const std::string Battery90;
 static const std::string BatteryAlert;
 static const std::string BatteryCharging;
 static const std::string BatteryCharging100;
 static const std::string BatteryCharging20;
 static const std::string BatteryCharging30;
 static const std::string BatteryCharging40;
 static const std::string BatteryCharging60;
 static const std::string BatteryCharging80;
 static const std::string BatteryCharging90;
 static const std::string BatteryMinus;
 static const std::string BatteryNegative;
 static const std::string BatteryOutline;
 static const std::string BatteryPlus;
 static const std::string BatteryPositive;
 static const std::string BatteryUnknown;
 static const std::string Beach;
 static const std::string Beaker;
 static const std::string Beats;
 static const std::string Beer;
 static const std::string Behance;
 static const std::string Bell;
 static const std::string BellOff;
 static const std::string BellOutline;
 static const std::string BellPlus;
 static const std::string BellRing;
 static const std::string BellRingOutline;
 static const std::string BellSleep;
 static const std::string Beta;
 static const std::string Bible;
 static const std::string Bike;
 static const std::string Bing;
 static const std::string Binoculars;
 static const std::string Bio;
 static const std::string Biohazard;
 static const std::string Bitbucket;
 static const std::string BlackMesa;
 static const std::string Blackberry;
 static const std::string Blender;
 static const std::string Blinds;
 static const std::string BlockHelper;
 static const std::string Blogger;
 static const std::string Bluetooth;
 static const std::string BluetoothAudio;
 static const std::string BluetoothConnect;
 static const std::string BluetoothOff;
 static const std::string BluetoothSettings;
 static const std::string BluetoothTransfer;
 static const std::string Blur;
 static const std::string BlurLinear;
 static const std::string BlurOff;
 static const std::string BlurRadial;
 static const std::string Bomb;
 static const std::string BombOff;
 static const std::string Bone;
 static const std::string Book;
 static const std::string BookMinus;
 static const std::string BookMultiple;
 static const std::string BookMultipleVariant;
 static const std::string BookOpen;
 static const std::string BookOpenPageVariant;
 static const std::string BookOpenVariant;
 static const std::string BookPlus;
 static const std::string BookVariant;
 static const std::string Bookmark;
 static const std::string BookmarkCheck;
 static const std::string BookmarkMusic;
 static const std::string BookmarkOutline;
 static const std::string BookmarkPlus;
 static const std::string BookmarkPlusOutline;
 static const std::string BookmarkRemove;
 static const std::string Boombox;
 static const std::string Bootstrap;
 static const std::string BorderAll;
 static const std::string BorderBottom;
 static const std::string BorderColor;
 static const std::string BorderHorizontal;
 static const std::string BorderInside;
 static const std::string BorderLeft;
 static const std::string BorderNone;
 static const std::string BorderOutside;
 static const std::string BorderRight;
 static const std::string BorderStyle;
 static const std::string BorderTop;
 static const std::string BorderVertical;
 static const std::string BowTie;
 static const std::string Bowl;
 static const std::string Bowling;
 static const std::string Box;
 static const std::string BoxCutter;
 static const std::string BoxShadow;
 static const std::string Bridge;
 static const std::string Briefcase;
 static const std::string BriefcaseCheck;
 static const std::string BriefcaseDownload;
 static const std::string BriefcaseUpload;
 static const std::string Brightness1;
 static const std::string Brightness2;
 static const std::string Brightness3;
 static const std::string Brightness4;
 static const std::string Brightness5;
 static const std::string Brightness6;
 static const std::string Brightness7;
 static const std::string BrightnessAuto;
 static const std::string Broom;
 static const std::string Brush;
 static const std::string Buffer;
 static const std::string Bug;
 static const std::string BulletinBoard;
 static const std::string Bullhorn;
 static const std::string Bullseye;
 static const std::string BurstMode;
 static const std::string Bus;
 static const std::string Cached;
 static const std::string Cake;
 static const std::string CakeLayered;
 static const std::string CakeVariant;
 static const std::string Calculator;
 static const std::string Calendar;
 static const std::string CalendarBlank;
 static const std::string CalendarCheck;
 static const std::string CalendarClock;
 static const std::string CalendarMultiple;
 static const std::string CalendarMultipleCheck;
 static const std::string CalendarPlus;
 static const std::string CalendarQuestion;
 static const std::string CalendarRange;
 static const std::string CalendarRemove;
 static const std::string CalendarText;
 static const std::string CalendarToday;
 static const std::string CallMade;
 static const std::string CallMerge;
 static const std::string CallMissed;
 static const std::string CallReceived;
 static const std::string CallSplit;
 static const std::string Camcorder;
 static const std::string CamcorderBox;
 static const std::string CamcorderBoxOff;
 static const std::string CamcorderOff;
 static const std::string Camera;
 static const std::string CameraBurst;
 static const std::string CameraEnhance;
 static const std::string CameraFront;
 static const std::string CameraFrontVariant;
 static const std::string CameraIris;
 static const std::string CameraOff;
 static const std::string CameraPartyMode;
 static const std::string CameraRear;
 static const std::string CameraRearVariant;
 static const std::string CameraSwitch;
 static const std::string CameraTimer;
 static const std::string Cancel;
 static const std::string Candle;
 static const std::string Candycane;
 static const std::string Car;
 static const std::string CarBattery;
 static const std::string CarConnected;
 static const std::string CarWash;
 static const std::string Cards;
 static const std::string CardsOutline;
 static const std::string CardsPlayingOutline;
 static const std::string CardsVariant;
 static const std::string Carrot;
 static const std::string Cart;
 static const std::string CartOff;
 static const std::string CartOutline;
 static const std::string CartPlus;
 static const std::string CaseSensitiveAlt;
 static const std::string Cash;
 static const std::string Cash100;
 static const std::string CashMultiple;
 static const std::string CashUsd;
 static const std::string Cast;
 static const std::string CastConnected;
 static const std::string CastOff;
 static const std::string Castle;
 static const std::string Cat;
 static const std::string CeilingLight;
 static const std::string Cellphone;
 static const std::string CellphoneAndroid;
 static const std::string CellphoneBasic;
 static const std::string CellphoneDock;
 static const std::string CellphoneIphone;
 static const std::string CellphoneLink;
 static const std::string CellphoneLinkOff;
 static const std::string CellphoneSettings;
 static const std::string Certificate;
 static const std::string ChairSchool;
 static const std::string ChartArc;
 static const std::string ChartAreaspline;
 static const std::string ChartBar;
 static const std::string ChartBarStacked;
 static const std::string ChartBubble;
 static const std::string ChartGantt;
 static const std::string ChartHistogram;
 static const std::string ChartLine;
 static const std::string ChartLineStacked;
 static const std::string ChartPie;
 static const std::string ChartScatterplotHexbin;
 static const std::string ChartTimeline;
 static const std::string Check;
 static const std::string CheckAll;
 static const std::string CheckCircle;
 static const std::string CheckCircleOutline;
 static const std::string CheckboxBlank;
 static const std::string CheckboxBlankCircle;
 static const std::string CheckboxBlankCircleOutline;
 static const std::string CheckboxBlankOutline;
 static const std::string CheckboxMarked;
 static const std::string CheckboxMarkedCircle;
 static const std::string CheckboxMarkedCircleOutline;
 static const std::string CheckboxMarkedOutline;
 static const std::string CheckboxMultipleBlank;
 static const std::string CheckboxMultipleBlankCircle;
 static const std::string CheckboxMultipleBlankCircleOutline;
 static const std::string CheckboxMultipleBlankOutline;
 static const std::string CheckboxMultipleMarked;
 static const std::string CheckboxMultipleMarkedCircle;
 static const std::string CheckboxMultipleMarkedCircleOutline;
 static const std::string CheckboxMultipleMarkedOutline;
 static const std::string Checkerboard;
 static const std::string ChemicalWeapon;
 static const std::string ChevronDoubleDown;
 static const std::string ChevronDoubleLeft;
 static const std::string ChevronDoubleRight;
 static const std::string ChevronDoubleUp;
 static const std::string ChevronDown;
 static const std::string ChevronLeft;
 static const std::string ChevronRight;
 static const std::string ChevronUp;
 static const std::string Chip;
 static const std::string Church;
 static const std::string Circle;
 static const std::string CircleOutline;
 static const std::string CiscoWebex;
 static const std::string City;
 static const std::string Clipboard;
 static const std::string ClipboardAccount;
 static const std::string ClipboardAlert;
 static const std::string ClipboardArrowDown;
 static const std::string ClipboardArrowLeft;
 static const std::string ClipboardCheck;
 static const std::string ClipboardFlow;
 static const std::string ClipboardOutline;
 static const std::string ClipboardPlus;
 static const std::string ClipboardText;
 static const std::string Clippy;
 static const std::string Clock;
 static const std::string ClockAlert;
 static const std::string ClockEnd;
 static const std::string ClockFast;
 static const std::string ClockIn;
 static const std::string ClockOut;
 static const std::string ClockStart;
 static const std::string Close;
 static const std::string CloseBox;
 static const std::string CloseBoxOutline;
 static const std::string CloseCircle;
 static const std::string CloseCircleOutline;
 static const std::string CloseNetwork;
 static const std::string CloseOctagon;
 static const std::string CloseOctagonOutline;
 static const std::string CloseOutline;
 static const std::string ClosedCaption;
 static const std::string Cloud;
 static const std::string CloudCheck;
 static const std::string CloudCircle;
 static const std::string CloudDownload;
 static const std::string CloudOffOutline;
 static const std::string CloudOutline;
 static const std::string CloudPrint;
 static const std::string CloudPrintOutline;
 static const std::string CloudSync;
 static const std::string CloudUpload;
 static const std::string CodeArray;
 static const std::string CodeBraces;
 static const std::string CodeBrackets;
 static const std::string CodeEqual;
 static const std::string CodeGreaterThan;
 static const std::string CodeGreaterThanOrEqual;
 static const std::string CodeLessThan;
 static const std::string CodeLessThanOrEqual;
 static const std::string CodeNotEqual;
 static const std::string CodeNotEqualVariant;
 static const std::string CodeParentheses;
 static const std::string CodeString;
 static const std::string CodeTags;
 static const std::string CodeTagsCheck;
 static const std::string Codepen;
 static const std::string Coffee;
 static const std::string CoffeeOutline;
 static const std::string CoffeeToGo;
 static const std::string Coin;
 static const std::string Coins;
 static const std::string Collage;
 static const std::string ColorHelper;
 static const std::string Comment;
 static const std::string CommentAccount;
 static const std::string CommentAccountOutline;
 static const std::string CommentAlert;
 static const std::string CommentAlertOutline;
 static const std::string CommentCheck;
 static const std::string CommentCheckOutline;
 static const std::string CommentMultipleOutline;
 static const std::string CommentOutline;
 static const std::string CommentPlusOutline;
 static const std::string CommentProcessing;
 static const std::string CommentProcessingOutline;
 static const std::string CommentQuestionOutline;
 static const std::string CommentRemoveOutline;
 static const std::string CommentText;
 static const std::string CommentTextOutline;
 static const std::string Compare;
 static const std::string Compass;
 static const std::string CompassOutline;
 static const std::string Console;
 static const std::string ContactMail;
 static const std::string Contacts;
 static const std::string ContentCopy;
 static const std::string ContentCut;
 static const std::string ContentDuplicate;
 static const std::string ContentPaste;
 static const std::string ContentSave;
 static const std::string ContentSaveAll;
 static const std::string ContentSaveSettings;
 static const std::string Contrast;
 static const std::string ContrastBox;
 static const std::string ContrastCircle;
 static const std::string Cookie;
 static const std::string Copyright;
 static const std::string Counter;
 static const std::string Cow;
 static const std::string Creation;
 static const std::string CreditCard;
 static const std::string CreditCardMultiple;
 static const std::string CreditCardOff;
 static const std::string CreditCardPlus;
 static const std::string CreditCardScan;
 static const std::string Crop;
 static const std::string CropFree;
 static const std::string CropLandscape;
 static const std::string CropPortrait;
 static const std::string CropRotate;
 static const std::string CropSquare;
 static const std::string Crosshairs;
 static const std::string CrosshairsGps;
 static const std::string Crown;
 static const std::string Cube;
 static const std::string CubeOutline;
 static const std::string CubeSend;
 static const std::string CubeUnfolded;
 static const std::string Cup;
 static const std::string CupOff;
 static const std::string CupWater;
 static const std::string CurrencyBtc;
 static const std::string CurrencyEur;
 static const std::string CurrencyGbp;
 static const std::string CurrencyInr;
 static const std::string CurrencyNgn;
 static const std::string CurrencyRub;
 static const std::string CurrencyTry;
 static const std::string CurrencyUsd;
 static const std::string CurrencyUsdOff;
 static const std::string CursorDefault;
 static const std::string CursorDefaultOutline;
 static const std::string CursorMove;
 static const std::string CursorPointer;
 static const std::string CursorText;
 static const std::string Database;
 static const std::string DatabaseMinus;
 static const std::string DatabasePlus;
 static const std::string DebugStepInto;
 static const std::string DebugStepOut;
 static const std::string DebugStepOver;
 static const std::string Decagram;
 static const std::string DecagramOutline;
 static const std::string DecimalDecrease;
 static const std::string DecimalIncrease;
 static const std::string Delete;
 static const std::string DeleteCircle;
 static const std::string DeleteEmpty;
 static const std::string DeleteForever;
 static const std::string DeleteSweep;
 static const std::string DeleteVariant;
 static const std::string Delta;
 static const std::string Deskphone;
 static const std::string DesktopMac;
 static const std::string DesktopTower;
 static const std::string Details;
 static const std::string DeveloperBoard;
 static const std::string Deviantart;
 static const std::string Dialpad;
 static const std::string Diamond;
 static const std::string Dice1;
 static const std::string Dice2;
 static const std::string Dice3;
 static const std::string Dice4;
 static const std::string Dice5;
 static const std::string Dice6;
 static const std::string DiceD10;
 static const std::string DiceD20;
 static const std::string DiceD4;
 static const std::string DiceD6;
 static const std::string DiceD8;
 static const std::string DiceMultiple;
 static const std::string Dictionary;
 static const std::string Directions;
 static const std::string DirectionsFork;
 static const std::string Discord;
 static const std::string Disk;
 static const std::string DiskAlert;
 static const std::string Disqus;
 static const std::string DisqusOutline;
 static const std::string Division;
 static const std::string DivisionBox;
 static const std::string Dna;
 static const std::string Dns;
 static const std::string DoNotDisturb;
 static const std::string DoNotDisturbOff;
 static const std::string Dolby;
 static const std::string Domain;
 static const std::string DotsHorizontal;
 static const std::string DotsVertical;
 static const std::string Douban;
 static const std::string Download;
 static const std::string DownloadNetwork;
 static const std::string Drag;
 static const std::string DragHorizontal;
 static const std::string DragVertical;
 static const std::string Drawing;
 static const std::string DrawingBox;
 static const std::string Dribbble;
 static const std::string DribbbleBox;
 static const std::string Drone;
 static const std::string Dropbox;
 static const std::string Drupal;
 static const std::string Duck;
 static const std::string Dumbbell;
 static const std::string Earth;
 static const std::string EarthBox;
 static const std::string EarthBoxOff;
 static const std::string EarthOff;
 static const std::string Edge;
 static const std::string Eject;
 static const std::string ElevationDecline;
 static const std::string ElevationRise;
 static const std::string Elevator;
 static const std::string Email;
 static const std::string EmailAlert;
 static const std::string EmailOpen;
 static const std::string EmailOpenOutline;
 static const std::string EmailOutline;
 static const std::string EmailSecure;
 static const std::string EmailVariant;
 static const std::string Emby;
 static const std::string Emoticon;
 static const std::string EmoticonCool;
 static const std::string EmoticonDead;
 static const std::string EmoticonDevil;
 static const std::string EmoticonExcited;
 static const std::string EmoticonHappy;
 static const std::string EmoticonNeutral;
 static const std::string EmoticonPoop;
 static const std::string EmoticonSad;
 static const std::string EmoticonTongue;
 static const std::string Engine;
 static const std::string EngineOutline;
 static const std::string Equal;
 static const std::string EqualBox;
 static const std::string Eraser;
 static const std::string EraserVariant;
 static const std::string Escalator;
 static const std::string Ethernet;
 static const std::string EthernetCable;
 static const std::string EthernetCableOff;
 static const std::string Etsy;
 static const std::string EvStation;
 static const std::string Evernote;
 static const std::string Exclamation;
 static const std::string ExitToApp;
 static const std::string Export;
 static const std::string Eye;
 static const std::string EyeOff;
 static const std::string EyeOffOutline;
 static const std::string EyeOutline;
 static const std::string Eyedropper;
 static const std::string EyedropperVariant;
 static const std::string Face;
 static const std::string FaceProfile;
 static const std::string Facebook;
 static const std::string FacebookBox;
 static const std::string FacebookMessenger;
 static const std::string Factory;
 static const std::string Fan;
 static const std::string FastForward;
 static const std::string FastForwardOutline;
 static const std::string Fax;
 static const std::string Feather;
 static const std::string Ferry;
 static const std::string File;
 static const std::string FileAccount;
 static const std::string FileChart;
 static const std::string FileCheck;
 static const std::string FileCloud;
 static const std::string FileDelimited;
 static const std::string FileDocument;
 static const std::string FileDocumentBox;
 static const std::string FileExcel;
 static const std::string FileExcelBox;
 static const std::string FileExport;
 static const std::string FileFind;
 static const std::string FileHidden;
 static const std::string FileImage;
 static const std::string FileImport;
 static const std::string FileLock;
 static const std::string FileMultiple;
 static const std::string FileMusic;
 static const std::string FileOutline;
 static const std::string FilePdf;
 static const std::string FilePdfBox;
 static const std::string FilePlus;
 static const std::string FilePowerpoint;
 static const std::string FilePowerpointBox;
 static const std::string FilePresentationBox;
 static const std::string FileRestore;
 static const std::string FileSend;
 static const std::string FileTree;
 static const std::string FileVideo;
 static const std::string FileWord;
 static const std::string FileWordBox;
 static const std::string FileXml;
 static const std::string Film;
 static const std::string Filmstrip;
 static const std::string FilmstripOff;
 static const std::string Filter;
 static const std::string FilterOutline;
 static const std::string FilterRemove;
 static const std::string FilterRemoveOutline;
 static const std::string FilterVariant;
 static const std::string FindReplace;
 static const std::string Fingerprint;
 static const std::string Fire;
 static const std::string Firefox;
 static const std::string Fish;
 static const std::string Flag;
 static const std::string FlagCheckered;
 static const std::string FlagOutline;
 static const std::string FlagOutlineVariant;
 static const std::string FlagTriangle;
 static const std::string FlagVariant;
 static const std::string Flash;
 static const std::string FlashAuto;
 static const std::string FlashOff;
 static const std::string FlashOutline;
 static const std::string FlashRedEye;
 static const std::string Flashlight;
 static const std::string FlashlightOff;
 static const std::string Flask;
 static const std::string FlaskEmpty;
 static const std::string FlaskEmptyOutline;
 static const std::string FlaskOutline;
 static const std::string Flattr;
 static const std::string FlipToBack;
 static const std::string FlipToFront;
 static const std::string Floppy;
 static const std::string Flower;
 static const std::string Folder;
 static const std::string FolderAccount;
 static const std::string FolderDownload;
 static const std::string FolderGoogleDrive;
 static const std::string FolderImage;
 static const std::string FolderLock;
 static const std::string FolderLockOpen;
 static const std::string FolderMove;
 static const std::string FolderMultiple;
 static const std::string FolderMultipleImage;
 static const std::string FolderMultipleOutline;
 static const std::string FolderOpen;
 static const std::string FolderOutline;
 static const std::string FolderPlus;
 static const std::string FolderRemove;
 static const std::string FolderStar;
 static const std::string FolderUpload;
 static const std::string FontAwesome;
 static const std::string Food;
 static const std::string FoodApple;
 static const std::string FoodForkDrink;
 static const std::string FoodOff;
 static const std::string FoodVariant;
 static const std::string Football;
 static const std::string FootballAustralian;
 static const std::string FootballHelmet;
 static const std::string FormatAlignBottom;
 static const std::string FormatAlignCenter;
 static const std::string FormatAlignJustify;
 static const std::string FormatAlignLeft;
 static const std::string FormatAlignMiddle;
 static const std::string FormatAlignRight;
 static const std::string FormatAlignTop;
 static const std::string FormatAnnotationPlus;
 static const std::string FormatBold;
 static const std::string FormatClear;
 static const std::string FormatColorFill;
 static const std::string FormatColorText;
 static const std::string FormatFloatCenter;
 static const std::string FormatFloatLeft;
 static const std::string FormatFloatNone;
 static const std::string FormatFloatRight;
 static const std::string FormatFont;
 static const std::string FormatHeader1;
 static const std::string FormatHeader2;
 static const std::string FormatHeader3;
 static const std::string FormatHeader4;
 static const std::string FormatHeader5;
 static const std::string FormatHeader6;
 static const std::string FormatHeaderDecrease;
 static const std::string FormatHeaderEqual;
 static const std::string FormatHeaderIncrease;
 static const std::string FormatHeaderPound;
 static const std::string FormatHorizontalAlignCenter;
 static const std::string FormatHorizontalAlignLeft;
 static const std::string FormatHorizontalAlignRight;
 static const std::string FormatIndentDecrease;
 static const std::string FormatIndentIncrease;
 static const std::string FormatItalic;
 static const std::string FormatLineSpacing;
 static const std::string FormatLineStyle;
 static const std::string FormatLineWeight;
 static const std::string FormatListBulleted;
 static const std::string FormatListBulletedType;
 static const std::string FormatListChecks;
 static const std::string FormatListNumbers;
 static const std::string FormatPageBreak;
 static const std::string FormatPaint;
 static const std::string FormatParagraph;
 static const std::string FormatPilcrow;
 static const std::string FormatQuoteClose;
 static const std::string FormatQuoteOpen;
 static const std::string FormatRotate90;
 static const std::string FormatSection;
 static const std::string FormatSize;
 static const std::string FormatStrikethrough;
 static const std::string FormatStrikethroughVariant;
 static const std::string FormatSubscript;
 static const std::string FormatSuperscript;
 static const std::string FormatText;
 static const std::string FormatTextdirectionLToR;
 static const std::string FormatTextdirectionRToL;
 static const std::string FormatTitle;
 static const std::string FormatUnderline;
 static const std::string FormatVerticalAlignBottom;
 static const std::string FormatVerticalAlignCenter;
 static const std::string FormatVerticalAlignTop;
 static const std::string FormatWrapInline;
 static const std::string FormatWrapSquare;
 static const std::string FormatWrapTight;
 static const std::string FormatWrapTopBottom;
 static const std::string Forum;
 static const std::string Forward;
 static const std::string Foursquare;
 static const std::string Fridge;
 static const std::string FridgeFilled;
 static const std::string FridgeFilledBottom;
 static const std::string FridgeFilledTop;
 static const std::string Fullscreen;
 static const std::string FullscreenExit;
 static const std::string Function;
 static const std::string Gamepad;
 static const std::string GamepadVariant;
 static const std::string Garage;
 static const std::string GarageOpen;
 static const std::string GasCylinder;
 static const std::string GasStation;
 static const std::string Gate;
 static const std::string Gauge;
 static const std::string Gavel;
 static const std::string GenderFemale;
 static const std::string GenderMale;
 static const std::string GenderMaleFemale;
 static const std::string GenderTransgender;
 static const std::string GestureDoubleTap;
 static const std::string GestureSwipeDown;
 static const std::string GestureSwipeLeft;
 static const std::string GestureSwipeRight;
 static const std::string GestureSwipeUp;
 static const std::string GestureTap;
 static const std::string GestureTwoDoubleTap;
 static const std::string GestureTwoTap;
 static const std::string Ghost;
 static const std::string Gift;
 static const std::string Git;
 static const std::string GithubBox;
 static const std::string GithubCircle;
 static const std::string GithubFace;
 static const std::string GlassFlute;
 static const std::string GlassMug;
 static const std::string GlassStange;
 static const std::string GlassTulip;
 static const std::string Glassdoor;
 static const std::string Glasses;
 static const std::string Gmail;
 static const std::string Gnome;
 static const std::string Gondola;
 static const std::string Google;
 static const std::string GoogleCardboard;
 static const std::string GoogleChrome;
 static const std::string GoogleCircles;
 static const std::string GoogleCirclesCommunities;
 static const std::string GoogleCirclesExtended;
 static const std::string GoogleCirclesGroup;
 static const std::string GoogleController;
 static const std::string GoogleControllerOff;
 static const std::string GoogleDrive;
 static const std::string GoogleEarth;
 static const std::string GoogleGlass;
 static const std::string GoogleKeep;
 static const std::string GoogleMaps;
 static const std::string GoogleNearby;
 static const std::string GooglePages;
 static const std::string GooglePhotos;
 static const std::string GooglePhysicalWeb;
 static const std::string GooglePlay;
 static const std::string GooglePlus;
 static const std::string GooglePlusBox;
 static const std::string GoogleTranslate;
 static const std::string GoogleWallet;
 static const std::string Gradient;
 static const std::string GreasePencil;
 static const std::string Grid;
 static const std::string GridLarge;
 static const std::string GridOff;
 static const std::string Group;
 static const std::string GuitarAcoustic;
 static const std::string GuitarElectric;
 static const std::string GuitarPick;
 static const std::string GuitarPickOutline;
 static const std::string Hackernews;
 static const std::string Hamburger;
 static const std::string HandPointingRight;
 static const std::string Hanger;
 static const std::string Hangouts;
 static const std::string Harddisk;
 static const std::string Headphones;
 static const std::string HeadphonesBox;
 static const std::string HeadphonesSettings;
 static const std::string Headset;
 static const std::string HeadsetDock;
 static const std::string HeadsetOff;
 static const std::string Heart;
 static const std::string HeartBox;
 static const std::string HeartBoxOutline;
 static const std::string HeartBroken;
 static const std::string HeartHalf;
 static const std::string HeartHalfFull;
 static const std::string HeartHalfOutline;
 static const std::string HeartOff;
 static const std::string HeartOutline;
 static const std::string HeartPulse;
 static const std::string Help;
 static const std::string HelpBox;
 static const std::string HelpCircle;
 static const std::string HelpCircleOutline;
 static const std::string HelpNetwork;
 static const std::string Hexagon;
 static const std::string HexagonMultiple;
 static const std::string HexagonOutline;
 static const std::string Highway;
 static const std::string History;
 static const std::string Hololens;
 static const std::string Home;
 static const std::string HomeMapMarker;
 static const std::string HomeModern;
 static const std::string HomeOutline;
 static const std::string HomeVariant;
 static const std::string Hook;
 static const std::string HookOff;
 static const std::string Hops;
 static const std::string Hospital;
 static const std::string HospitalBuilding;
 static const std::string HospitalMarker;
 static const std::string Hotel;
 static const std::string Houzz;
 static const std::string HouzzBox;
 static const std::string Human;
 static const std::string HumanChild;
 static const std::string HumanFemale;
 static const std::string HumanGreeting;
 static const std::string HumanHandsdown;
 static const std::string HumanHandsup;
 static const std::string HumanMale;
 static const std::string HumanMaleFemale;
 static const std::string HumanPregnant;
 static const std::string HumbleBundle;
 static const std::string Image;
 static const std::string ImageAlbum;
 static const std::string ImageArea;
 static const std::string ImageAreaClose;
 static const std::string ImageBroken;
 static const std::string ImageBrokenVariant;
 static const std::string ImageFilter;
 static const std::string ImageFilterBlackWhite;
 static const std::string ImageFilterCenterFocus;
 static const std::string ImageFilterCenterFocusWeak;
 static const std::string ImageFilterDrama;
 static const std::string ImageFilterFrames;
 static const std::string ImageFilterHdr;
 static const std::string ImageFilterNone;
 static const std::string ImageFilterTiltShift;
 static const std::string ImageFilterVintage;
 static const std::string ImageMultiple;
 static const std::string Import;
 static const std::string Inbox;
 static const std::string InboxArrowDown;
 static const std::string InboxArrowUp;
 static const std::string Incognito;
 static const std::string Infinity;
 static const std::string Information;
 static const std::string InformationOutline;
 static const std::string InformationVariant;
 static const std::string Instagram;
 static const std::string Instapaper;
 static const std::string InternetExplorer;
 static const std::string InvertColors;
 static const std::string Itunes;
 static const std::string Jeepney;
 static const std::string Jira;
 static const std::string Jsfiddle;
 static const std::string Json;
 static const std::string Keg;
 static const std::string Kettle;
 static const std::string Key;
 static const std::string KeyChange;
 static const std::string KeyMinus;
 static const std::string KeyPlus;
 static const std::string KeyRemove;
 static const std::string KeyVariant;
 static const std::string Keyboard;
 static const std::string KeyboardBackspace;
 static const std::string KeyboardCaps;
 static const std::string KeyboardClose;
 static const std::string KeyboardOff;
 static const std::string KeyboardReturn;
 static const std::string KeyboardTab;
 static const std::string KeyboardVariant;
 static const std::string Kickstarter;
 static const std::string Kodi;
 static const std::string Label;
 static const std::string LabelOutline;
 static const std::string Lambda;
 static const std::string Lamp;
 static const std::string Lan;
 static const std::string LanConnect;
 static const std::string LanDisconnect;
 static const std::string LanPending;
 static const std::string LanguageC;
 static const std::string LanguageCpp;
 static const std::string LanguageCsharp;
 static const std::string LanguageCss3;
 static const std::string LanguageHtml5;
 static const std::string LanguageJavascript;
 static const std::string LanguagePhp;
 static const std::string LanguagePython;
 static const std::string LanguagePythonText;
 static const std::string LanguageSwift;
 static const std::string LanguageTypescript;
 static const std::string Laptop;
 static const std::string LaptopChromebook;
 static const std::string LaptopMac;
 static const std::string LaptopOff;
 static const std::string LaptopWindows;
 static const std::string Lastfm;
 static const std::string Launch;
 static const std::string Layers;
 static const std::string LayersOff;
 static const std::string LeadPencil;
 static const std::string Leaf;
 static const std::string LedOff;
 static const std::string LedOn;
 static const std::string LedOutline;
 static const std::string LedVariantOff;
 static const std::string LedVariantOn;
 static const std::string LedVariantOutline;
 static const std::string Library;
 static const std::string LibraryBooks;
 static const std::string LibraryMusic;
 static const std::string LibraryPlus;
 static const std::string Lightbulb;
 static const std::string LightbulbOn;
 static const std::string LightbulbOnOutline;
 static const std::string LightbulbOutline;
 static const std::string Link;
 static const std::string LinkOff;
 static const std::string LinkVariant;
 static const std::string LinkVariantOff;
 static const std::string Linkedin;
 static const std::string LinkedinBox;
 static const std::string Linux;
 static const std::string Loading;
 static const std::string Lock;
 static const std::string LockOpen;
 static const std::string LockOpenOutline;
 static const std::string LockOutline;
 static const std::string LockPattern;
 static const std::string LockPlus;
 static const std::string LockReset;
 static const std::string Login;
 static const std::string LoginVariant;
 static const std::string Logout;
 static const std::string LogoutVariant;
 static const std::string Looks;
 static const std::string Loop;
 static const std::string Loupe;
 static const std::string Lumx;
 static const std::string Magnet;
 static const std::string MagnetOn;
 static const std::string Magnify;
 static const std::string MagnifyMinus;
 static const std::string MagnifyMinusOutline;
 static const std::string MagnifyPlus;
 static const std::string MagnifyPlusOutline;
 static const std::string MailRu;
 static const std::string Mailbox;
 static const std::string Map;
 static const std::string MapMarker;
 static const std::string MapMarkerCircle;
 static const std::string MapMarkerMinus;
 static const std::string MapMarkerMultiple;
 static const std::string MapMarkerOff;
 static const std::string MapMarkerPlus;
 static const std::string MapMarkerRadius;
 static const std::string Margin;
 static const std::string Markdown;
 static const std::string Marker;
 static const std::string MarkerCheck;
 static const std::string Martini;
 static const std::string MaterialUi;
 static const std::string MathCompass;
 static const std::string Matrix;
 static const std::string Maxcdn;
 static const std::string MedicalBag;
 static const std::string Medium;
 static const std::string Memory;
 static const std::string Menu;
 static const std::string MenuDown;
 static const std::string MenuDownOutline;
 static const std::string MenuLeft;
 static const std::string MenuRight;
 static const std::string MenuUp;
 static const std::string MenuUpOutline;
 static const std::string Message;
 static const std::string MessageAlert;
 static const std::string MessageBulleted;
 static const std::string MessageBulletedOff;
 static const std::string MessageDraw;
 static const std::string MessageImage;
 static const std::string MessageOutline;
 static const std::string MessagePlus;
 static const std::string MessageProcessing;
 static const std::string MessageReply;
 static const std::string MessageReplyText;
 static const std::string MessageSettings;
 static const std::string MessageSettingsVariant;
 static const std::string MessageText;
 static const std::string MessageTextOutline;
 static const std::string MessageVideo;
 static const std::string Meteor;
 static const std::string Microphone;
 static const std::string MicrophoneOff;
 static const std::string MicrophoneOutline;
 static const std::string MicrophoneSettings;
 static const std::string MicrophoneVariant;
 static const std::string MicrophoneVariantOff;
 static const std::string Microscope;
 static const std::string Microsoft;
 static const std::string Minecraft;
 static const std::string Minus;
 static const std::string MinusBox;
 static const std::string MinusBoxOutline;
 static const std::string MinusCircle;
 static const std::string MinusCircleOutline;
 static const std::string MinusNetwork;
 static const std::string Mixcloud;
 static const std::string Monitor;
 static const std::string MonitorMultiple;
 static const std::string More;
 static const std::string Motorbike;
 static const std::string Mouse;
 static const std::string MouseOff;
 static const std::string MouseVariant;
 static const std::string MouseVariantOff;
 static const std::string MoveResize;
 static const std::string MoveResizeVariant;
 static const std::string Movie;
 static const std::string Multiplication;
 static const std::string MultiplicationBox;
 static const std::string Music;
 static const std::string MusicBox;
 static const std::string MusicBoxOutline;
 static const std::string MusicCircle;
 static const std::string MusicNote;
 static const std::string MusicNoteBluetooth;
 static const std::string MusicNoteBluetoothOff;
 static const std::string MusicNoteEighth;
 static const std::string MusicNoteHalf;
 static const std::string MusicNoteOff;
 static const std::string MusicNoteQuarter;
 static const std::string MusicNoteSixteenth;
 static const std::string MusicNoteWhole;
 static const std::string MusicOff;
 static const std::string Nature;
 static const std::string NaturePeople;
 static const std::string Navigation;
 static const std::string NearMe;
 static const std::string Needle;
 static const std::string NestProtect;
 static const std::string NestThermostat;
 static const std::string Netflix;
 static const std::string Network;
 static const std::string NewBox;
 static const std::string Newspaper;
 static const std::string Nfc;
 static const std::string NfcTap;
 static const std::string NfcVariant;
 static const std::string Ninja;
 static const std::string Nodejs;
 static const std::string Note;
 static const std::string NoteMultiple;
 static const std::string NoteMultipleOutline;
 static const std::string NoteOutline;
 static const std::string NotePlus;
 static const std::string NotePlusOutline;
 static const std::string NoteText;
 static const std::string NotificationClearAll;
 static const std::string Npm;
 static const std::string Nuke;
 static const std::string Numeric;
 static const std::string Numeric0Box;
 static const std::string Numeric0BoxMultipleOutline;
 static const std::string Numeric0BoxOutline;
 static const std::string Numeric1Box;
 static const std::string Numeric1BoxMultipleOutline;
 static const std::string Numeric1BoxOutline;
 static const std::string Numeric2Box;
 static const std::string Numeric2BoxMultipleOutline;
 static const std::string Numeric2BoxOutline;
 static const std::string Numeric3Box;
 static const std::string Numeric3BoxMultipleOutline;
 static const std::string Numeric3BoxOutline;
 static const std::string Numeric4Box;
 static const std::string Numeric4BoxMultipleOutline;
 static const std::string Numeric4BoxOutline;
 static const std::string Numeric5Box;
 static const std::string Numeric5BoxMultipleOutline;
 static const std::string Numeric5BoxOutline;
 static const std::string Numeric6Box;
 static const std::string Numeric6BoxMultipleOutline;
 static const std::string Numeric6BoxOutline;
 static const std::string Numeric7Box;
 static const std::string Numeric7BoxMultipleOutline;
 static const std::string Numeric7BoxOutline;
 static const std::string Numeric8Box;
 static const std::string Numeric8BoxMultipleOutline;
 static const std::string Numeric8BoxOutline;
 static const std::string Numeric9Box;
 static const std::string Numeric9BoxMultipleOutline;
 static const std::string Numeric9BoxOutline;
 static const std::string Numeric9PlusBox;
 static const std::string Numeric9PlusBoxMultipleOutline;
 static const std::string Numeric9PlusBoxOutline;
 static const std::string Nut;
 static const std::string Nutrition;
 static const std::string Oar;
 static const std::string Octagon;
 static const std::string OctagonOutline;
 static const std::string Octagram;
 static const std::string OctagramOutline;
 static const std::string Odnoklassniki;
 static const std::string Office;
 static const std::string Oil;
 static const std::string OilTemperature;
 static const std::string Omega;
 static const std::string Onedrive;
 static const std::string Onenote;
 static const std::string Opacity;
 static const std::string OpenInApp;
 static const std::string OpenInNew;
 static const std::string Openid;
 static const std::string Opera;
 static const std::string Orbit;
 static const std::string Ornament;
 static const std::string OrnamentVariant;
 static const std::string Owl;
 static const std::string Package;
 static const std::string PackageDown;
 static const std::string PackageUp;
 static const std::string PackageVariant;
 static const std::string PackageVariantClosed;
 static const std::string PageFirst;
 static const std::string PageLast;
 static const std::string PageLayoutBody;
 static const std::string PageLayoutFooter;
 static const std::string PageLayoutHeader;
 static const std::string PageLayoutSidebarLeft;
 static const std::string PageLayoutSidebarRight;
 static const std::string Palette;
 static const std::string PaletteAdvanced;
 static const std::string Panda;
 static const std::string Pandora;
 static const std::string Panorama;
 static const std::string PanoramaFisheye;
 static const std::string PanoramaHorizontal;
 static const std::string PanoramaVertical;
 static const std::string PanoramaWideAngle;
 static const std::string PaperCutVertical;
 static const std::string Paperclip;
 static const std::string Parking;
 static const std::string Pause;
 static const std::string PauseCircle;
 static const std::string PauseCircleOutline;
 static const std::string PauseOctagon;
 static const std::string PauseOctagonOutline;
 static const std::string Paw;
 static const std::string PawOff;
 static const std::string Pen;
 static const std::string Pencil;
 static const std::string PencilBox;
 static const std::string PencilBoxOutline;
 static const std::string PencilCircle;
 static const std::string PencilCircleOutline;
 static const std::string PencilLock;
 static const std::string PencilOff;
 static const std::string Pentagon;
 static const std::string PentagonOutline;
 static const std::string Percent;
 static const std::string Periscope;
 static const std::string Pharmacy;
 static const std::string Phone;
 static const std::string PhoneBluetooth;
 static const std::string PhoneClassic;
 static const std::string PhoneForward;
 static const std::string PhoneHangup;
 static const std::string PhoneInTalk;
 static const std::string PhoneIncoming;
 static const std::string PhoneLocked;
 static const std::string PhoneLog;
 static const std::string PhoneMinus;
 static const std::string PhoneMissed;
 static const std::string PhoneOutgoing;
 static const std::string PhonePaused;
 static const std::string PhonePlus;
 static const std::string PhoneSettings;
 static const std::string PhoneVoip;
 static const std::string Pi;
 static const std::string PiBox;
 static const std::string Piano;
 static const std::string Pig;
 static const std::string Pill;
 static const std::string Pillar;
 static const std::string Pin;
 static const std::string PinOff;
 static const std::string PineTree;
 static const std::string PineTreeBox;
 static const std::string Pinterest;
 static const std::string PinterestBox;
 static const std::string Pistol;
 static const std::string Pizza;
 static const std::string PlaneShield;
 static const std::string Play;
 static const std::string PlayBoxOutline;
 static const std::string PlayCircle;
 static const std::string PlayCircleOutline;
 static const std::string PlayPause;
 static const std::string PlayProtectedContent;
 static const std::string PlaylistCheck;
 static const std::string PlaylistMinus;
 static const std::string PlaylistPlay;
 static const std::string PlaylistPlus;
 static const std::string PlaylistRemove;
 static const std::string Playstation;
 static const std::string Plex;
 static const std::string Plus;
 static const std::string PlusBox;
 static const std::string PlusBoxOutline;
 static const std::string PlusCircle;
 static const std::string PlusCircleMultipleOutline;
 static const std::string PlusCircleOutline;
 static const std::string PlusNetwork;
 static const std::string PlusOne;
 static const std::string PlusOutline;
 static const std::string Pocket;
 static const std::string Pokeball;
 static const std::string Polaroid;
 static const std::string Poll;
 static const std::string PollBox;
 static const std::string Polymer;
 static const std::string Pool;
 static const std::string Popcorn;
 static const std::string Pot;
 static const std::string PotMix;
 static const std::string Pound;
 static const std::string PoundBox;
 static const std::string Power;
 static const std::string PowerPlug;
 static const std::string PowerPlugOff;
 static const std::string PowerSettings;
 static const std::string PowerSocket;
 static const std::string Prescription;
 static const std::string Presentation;
 static const std::string PresentationPlay;
 static const std::string Printer;
 static const std::string Printer3d;
 static const std::string PrinterAlert;
 static const std::string PrinterSettings;
 static const std::string PriorityHigh;
 static const std::string PriorityLow;
 static const std::string ProfessionalHexagon;
 static const std::string Projector;
 static const std::string ProjectorScreen;
 static const std::string Publish;
 static const std::string Pulse;
 static const std::string Puzzle;
 static const std::string Qqchat;
 static const std::string Qrcode;
 static const std::string QrcodeScan;
 static const std::string Quadcopter;
 static const std::string QualityHigh;
 static const std::string Quicktime;
 static const std::string Radar;
 static const std::string Radiator;
 static const std::string Radio;
 static const std::string RadioHandheld;
 static const std::string RadioTower;
 static const std::string Radioactive;
 static const std::string RadioboxBlank;
 static const std::string RadioboxMarked;
 static const std::string Raspberrypi;
 static const std::string RayEnd;
 static const std::string RayEndArrow;
 static const std::string RayStart;
 static const std::string RayStartArrow;
 static const std::string RayStartEnd;
 static const std::string RayVertex;
 static const std::string Rdio;
 static const std::string React;
 static const std::string Read;
 static const std::string Readability;
 static const std::string Receipt;
 static const std::string Record;
 static const std::string RecordRec;
 static const std::string Recycle;
 static const std::string Reddit;
 static const std::string Redo;
 static const std::string RedoVariant;
 static const std::string Refresh;
 static const std::string Regex;
 static const std::string RelativeScale;
 static const std::string Reload;
 static const std::string Remote;
 static const std::string RenameBox;
 static const std::string ReorderHorizontal;
 static const std::string ReorderVertical;
 static const std::string Repeat;
 static const std::string RepeatOff;
 static const std::string RepeatOnce;
 static const std::string Replay;
 static const std::string Reply;
 static const std::string ReplyAll;
 static const std::string Reproduction;
 static const std::string ResizeBottomRight;
 static const std::string Responsive;
 static const std::string Restart;
 static const std::string Restore;
 static const std::string Rewind;
 static const std::string RewindOutline;
 static const std::string Rhombus;
 static const std::string RhombusOutline;
 static const std::string Ribbon;
 static const std::string Road;
 static const std::string RoadVariant;
 static const std::string Robot;
 static const std::string Rocket;
 static const std::string Roomba;
 static const std::string Rotate3d;
 static const std::string RotateLeft;
 static const std::string RotateLeftVariant;
 static const std::string RotateRight;
 static const std::string RotateRightVariant;
 static const std::string RoundedCorner;
 static const std::string RouterWireless;
 static const std::string Routes;
 static const std::string Rowing;
 static const std::string Rss;
 static const std::string RssBox;
 static const std::string Ruler;
 static const std::string Run;
 static const std::string RunFast;
 static const std::string Sale;
 static const std::string Satellite;
 static const std::string SatelliteVariant;
 static const std::string Saxophone;
 static const std::string Scale;
 static const std::string ScaleBalance;
 static const std::string ScaleBathroom;
 static const std::string Scanner;
 static const std::string School;
 static const std::string ScreenRotation;
 static const std::string ScreenRotationLock;
 static const std::string Screwdriver;
 static const std::string Script;
 static const std::string Sd;
 static const std::string Seal;
 static const std::string SearchWeb;
 static const std::string SeatFlat;
 static const std::string SeatFlatAngled;
 static const std::string SeatIndividualSuite;
 static const std::string SeatLegroomExtra;
 static const std::string SeatLegroomNormal;
 static const std::string SeatLegroomReduced;
 static const std::string SeatReclineExtra;
 static const std::string SeatReclineNormal;
 static const std::string Security;
 static const std::string SecurityHome;
 static const std::string SecurityNetwork;
 static const std::string Select;
 static const std::string SelectAll;
 static const std::string SelectInverse;
 static const std::string SelectOff;
 static const std::string Selection;
 static const std::string SelectionOff;
 static const std::string Send;
 static const std::string SerialPort;
 static const std::string Server;
 static const std::string ServerMinus;
 static const std::string ServerNetwork;
 static const std::string ServerNetworkOff;
 static const std::string ServerOff;
 static const std::string ServerPlus;
 static const std::string ServerRemove;
 static const std::string ServerSecurity;
 static const std::string SetAll;
 static const std::string SetCenter;
 static const std::string SetCenterRight;
 static const std::string SetLeft;
 static const std::string SetLeftCenter;
 static const std::string SetLeftRight;
 static const std::string SetNone;
 static const std::string SetRight;
 static const std::string Settings;
 static const std::string SettingsBox;
 static const std::string ShapeCirclePlus;
 static const std::string ShapePlus;
 static const std::string ShapePolygonPlus;
 static const std::string ShapeRectanglePlus;
 static const std::string ShapeSquarePlus;
 static const std::string Share;
 static const std::string ShareVariant;
 static const std::string Shield;
 static const std::string ShieldHalfFull;
 static const std::string ShieldOutline;
 static const std::string Shopping;
 static const std::string ShoppingMusic;
 static const std::string Shovel;
 static const std::string ShovelOff;
 static const std::string Shredder;
 static const std::string Shuffle;
 static const std::string ShuffleDisabled;
 static const std::string ShuffleVariant;
 static const std::string Sigma;
 static const std::string SigmaLower;
 static const std::string SignCaution;
 static const std::string SignDirection;
 static const std::string SignText;
 static const std::string Signal;
 static const std::string Signal2g;
 static const std::string Signal3g;
 static const std::string Signal4g;
 static const std::string SignalHspa;
 static const std::string SignalHspaPlus;
 static const std::string SignalOff;
 static const std::string SignalVariant;
 static const std::string Silverware;
 static const std::string SilverwareFork;
 static const std::string SilverwareSpoon;
 static const std::string SilverwareVariant;
 static const std::string Sim;
 static const std::string SimAlert;
 static const std::string SimOff;
 static const std::string Sitemap;
 static const std::string SkipBackward;
 static const std::string SkipForward;
 static const std::string SkipNext;
 static const std::string SkipNextCircle;
 static const std::string SkipNextCircleOutline;
 static const std::string SkipPrevious;
 static const std::string SkipPreviousCircle;
 static const std::string SkipPreviousCircleOutline;
 static const std::string Skull;
 static const std::string Skype;
 static const std::string SkypeBusiness;
 static const std::string Slack;
 static const std::string Sleep;
 static const std::string SleepOff;
 static const std::string Smoking;
 static const std::string SmokingOff;
 static const std::string Snapchat;
 static const std::string Snowflake;
 static const std::string Snowman;
 static const std::string Soccer;
 static const std::string Sofa;
 static const std::string Solid;
 static const std::string Sort;
 static const std::string SortAlphabetical;
 static const std::string SortAscending;
 static const std::string SortDescending;
 static const std::string SortNumeric;
 static const std::string SortVariant;
 static const std::string Soundcloud;
 static const std::string SourceBranch;
 static const std::string SourceCommit;
 static const std::string SourceCommitEnd;
 static const std::string SourceCommitEndLocal;
 static const std::string SourceCommitLocal;
 static const std::string SourceCommitNextLocal;
 static const std::string SourceCommitStart;
 static const std::string SourceCommitStartNextLocal;
 static const std::string SourceFork;
 static const std::string SourceMerge;
 static const std::string SourcePull;
 static const std::string Speaker;
 static const std::string SpeakerOff;
 static const std::string SpeakerWireless;
 static const std::string Speedometer;
 static const std::string Spellcheck;
 static const std::string Spotify;
 static const std::string Spotlight;
 static const std::string SpotlightBeam;
 static const std::string Spray;
 static const std::string Square;
 static const std::string SquareInc;
 static const std::string SquareIncCash;
 static const std::string SquareOutline;
 static const std::string SquareRoot;
 static const std::string Stackexchange;
 static const std::string Stackoverflow;
 static const std::string Stadium;
 static const std::string Stairs;
 static const std::string Star;
 static const std::string StarCircle;
 static const std::string StarHalf;
 static const std::string StarOff;
 static const std::string StarOutline;
 static const std::string Steam;
 static const std::string Steering;
 static const std::string StepBackward;
 static const std::string StepBackward2;
 static const std::string StepForward;
 static const std::string StepForward2;
 static const std::string Stethoscope;
 static const std::string Sticker;
 static const std::string StickerEmoji;
 static const std::string Stocking;
 static const std::string Stop;
 static const std::string StopCircle;
 static const std::string StopCircleOutline;
 static const std::string Store;
 static const std::string Store24Hour;
 static const std::string Stove;
 static const std::string SubdirectoryArrowLeft;
 static const std::string SubdirectoryArrowRight;
 static const std::string Subway;
 static const std::string SubwayVariant;
 static const std::string Summit;
 static const std::string Sunglasses;
 static const std::string SurroundSound;
 static const std::string Svg;
 static const std::string SwapHorizontal;
 static const std::string SwapVertical;
 static const std::string Swim;
 static const std::string Switch;
 static const std::string Sword;
 static const std::string SwordCross;
 static const std::string Sync;
 static const std::string SyncAlert;
 static const std::string SyncOff;
 static const std::string Tab;
 static const std::string TabPlus;
 static const std::string TabUnselected;
 static const std::string Table;
 static const std::string TableColumnPlusAfter;
 static const std::string TableColumnPlusBefore;
 static const std::string TableColumnRemove;
 static const std::string TableColumnWidth;
 static const std::string TableEdit;
 static const std::string TableLarge;
 static const std::string TableRowHeight;
 static const std::string TableRowPlusAfter;
 static const std::string TableRowPlusBefore;
 static const std::string TableRowRemove;
 static const std::string Tablet;
 static const std::string TabletAndroid;
 static const std::string TabletIpad;
 static const std::string Taco;
 static const std::string Tag;
 static const std::string TagFaces;
 static const std::string TagHeart;
 static const std::string TagMultiple;
 static const std::string TagOutline;
 static const std::string TagPlus;
 static const std::string TagRemove;
 static const std::string TagTextOutline;
 static const std::string Target;
 static const std::string Taxi;
 static const std::string Teamviewer;
 static const std::string Telegram;
 static const std::string Television;
 static const std::string TelevisionGuide;
 static const std::string TemperatureCelsius;
 static const std::string TemperatureFahrenheit;
 static const std::string TemperatureKelvin;
 static const std::string Tennis;
 static const std::string Tent;
 static const std::string Terrain;
 static const std::string TestTube;
 static const std::string TextShadow;
 static const std::string TextToSpeech;
 static const std::string TextToSpeechOff;
 static const std::string Textbox;
 static const std::string Texture;
 static const std::string Theater;
 static const std::string ThemeLightDark;
 static const std::string Thermometer;
 static const std::string ThermometerLines;
 static const std::string ThumbDown;
 static const std::string ThumbDownOutline;
 static const std::string ThumbUp;
 static const std::string ThumbUpOutline;
 static const std::string ThumbsUpDown;
 static const std::string Ticket;
 static const std::string TicketAccount;
 static const std::string TicketConfirmation;
 static const std::string TicketPercent;
 static const std::string Tie;
 static const std::string Tilde;
 static const std::string Timelapse;
 static const std::string Timer;
 static const std::string Timer10;
 static const std::string Timer3;
 static const std::string TimerOff;
 static const std::string TimerSand;
 static const std::string TimerSandEmpty;
 static const std::string TimerSandFull;
 static const std::string Timetable;
 static const std::string ToggleSwitch;
 static const std::string ToggleSwitchOff;
 static const std::string Tooltip;
 static const std::string TooltipEdit;
 static const std::string TooltipImage;
 static const std::string TooltipOutline;
 static const std::string TooltipOutlinePlus;
 static const std::string TooltipText;
 static const std::string Tooth;
 static const std::string Tor;
 static const std::string TowerBeach;
 static const std::string TowerFire;
 static const std::string TrafficLight;
 static const std::string Train;
 static const std::string Tram;
 static const std::string Transcribe;
 static const std::string TranscribeClose;
 static const std::string Transfer;
 static const std::string TransitTransfer;
 static const std::string Translate;
 static const std::string TreasureChest;
 static const std::string Tree;
 static const std::string Trello;
 static const std::string TrendingDown;
 static const std::string TrendingNeutral;
 static const std::string TrendingUp;
 static const std::string Triangle;
 static const std::string TriangleOutline;
 static const std::string Trophy;
 static const std::string TrophyAward;
 static const std::string TrophyOutline;
 static const std::string TrophyVariant;
 static const std::string TrophyVariantOutline;
 static const std::string Truck;
 static const std::string TruckDelivery;
 static const std::string TruckFast;
 static const std::string TruckTrailer;
 static const std::string TshirtCrew;
 static const std::string TshirtV;
 static const std::string Tumblr;
 static const std::string TumblrReblog;
 static const std::string Tune;
 static const std::string TuneVertical;
 static const std::string Twitch;
 static const std::string Twitter;
 static const std::string TwitterBox;
 static const std::string TwitterCircle;
 static const std::string TwitterRetweet;
 static const std::string Uber;
 static const std::string Ubuntu;
 static const std::string Umbraco;
 static const std::string Umbrella;
 static const std::string UmbrellaOutline;
 static const std::string Undo;
 static const std::string UndoVariant;
 static const std::string UnfoldLessHorizontal;
 static const std::string UnfoldLessVertical;
 static const std::string UnfoldMoreHorizontal;
 static const std::string UnfoldMoreVertical;
 static const std::string Ungroup;
 static const std::string Unity;
 static const std::string Untappd;
 static const std::string Update;
 static const std::string Upload;
 static const std::string UploadNetwork;
 static const std::string Usb;
 static const std::string VectorArrangeAbove;
 static const std::string VectorArrangeBelow;
 static const std::string VectorCircle;
 static const std::string VectorCircleVariant;
 static const std::string VectorCombine;
 static const std::string VectorCurve;
 static const std::string VectorDifference;
 static const std::string VectorDifferenceAb;
 static const std::string VectorDifferenceBa;
 static const std::string VectorIntersection;
 static const std::string VectorLine;
 static const std::string VectorPoint;
 static const std::string VectorPolygon;
 static const std::string VectorPolyline;
 static const std::string VectorRadius;
 static const std::string VectorRectangle;
 static const std::string VectorSelection;
 static const std::string VectorSquare;
 static const std::string VectorTriangle;
 static const std::string VectorUnion;
 static const std::string Verified;
 static const std::string Vibrate;
 static const std::string Video;
 static const std::string VideoOff;
 static const std::string VideoSwitch;
 static const std::string ViewAgenda;
 static const std::string ViewArray;
 static const std::string ViewCarousel;
 static const std::string ViewColumn;
 static const std::string ViewDashboard;
 static const std::string ViewDay;
 static const std::string ViewGrid;
 static const std::string ViewHeadline;
 static const std::string ViewList;
 static const std::string ViewModule;
 static const std::string ViewParallel;
 static const std::string ViewQuilt;
 static const std::string ViewSequential;
 static const std::string ViewStream;
 static const std::string ViewWeek;
 static const std::string Vimeo;
 static const std::string Vine;
 static const std::string Violin;
 static const std::string Visualstudio;
 static const std::string Vk;
 static const std::string VkBox;
 static const std::string VkCircle;
 static const std::string Vlc;
 static const std::string Voice;
 static const std::string Voicemail;
 static const std::string VolumeHigh;
 static const std::string VolumeLow;
 static const std::string VolumeMedium;
 static const std::string VolumeMinus;
 static const std::string VolumeMute;
 static const std::string VolumeOff;
 static const std::string VolumePlus;
 static const std::string Vpn;
 static const std::string Walk;
 static const std::string Wallet;
 static const std::string WalletGiftcard;
 static const std::string WalletMembership;
 static const std::string WalletTravel;
 static const std::string Wan;
 static const std::string WashingMachine;
 static const std::string Watch;
 static const std::string WatchExport;
 static const std::string WatchImport;
 static const std::string WatchVibrate;
 static const std::string Water;
 static const std::string WaterOff;
 static const std::string WaterPercent;
 static const std::string WaterPump;
 static const std::string Watermark;
 static const std::string Waves;
 static const std::string WeatherCloudy;
 static const std::string WeatherFog;
 static const std::string WeatherHail;
 static const std::string WeatherLightning;
 static const std::string WeatherLightningRainy;
 static const std::string WeatherNight;
 static const std::string WeatherPartlycloudy;
 static const std::string WeatherPouring;
 static const std::string WeatherRainy;
 static const std::string WeatherSnowy;
 static const std::string WeatherSnowyRainy;
 static const std::string WeatherSunny;
 static const std::string WeatherSunset;
 static const std::string WeatherSunsetDown;
 static const std::string WeatherSunsetUp;
 static const std::string WeatherWindy;
 static const std::string WeatherWindyVariant;
 static const std::string Web;
 static const std::string Webcam;
 static const std::string Webhook;
 static const std::string Webpack;
 static const std::string Wechat;
 static const std::string Weight;
 static const std::string WeightKilogram;
 static const std::string Whatsapp;
 static const std::string WheelchairAccessibility;
 static const std::string WhiteBalanceAuto;
 static const std::string WhiteBalanceIncandescent;
 static const std::string WhiteBalanceIridescent;
 static const std::string WhiteBalanceSunny;
 static const std::string Widgets;
 static const std::string Wifi;
 static const std::string WifiOff;
 static const std::string Wii;
 static const std::string Wiiu;
 static const std::string Wikipedia;
 static const std::string WindowClose;
 static const std::string WindowClosed;
 static const std::string WindowMaximize;
 static const std::string WindowMinimize;
 static const std::string WindowOpen;
 static const std::string WindowRestore;
 static const std::string Windows;
 static const std::string Wordpress;
 static const std::string Worker;
 static const std::string Wrap;
 static const std::string Wrench;
 static const std::string Wunderlist;
 static const std::string Xaml;
 static const std::string Xbox;
 static const std::string XboxController;
 static const std::string XboxControllerBatteryAlert;
 static const std::string XboxControllerBatteryEmpty;
 static const std::string XboxControllerBatteryFull;
 static const std::string XboxControllerBatteryLow;
 static const std::string XboxControllerBatteryMedium;
 static const std::string XboxControllerBatteryUnknown;
 static const std::string XboxControllerOff;
 static const std::string Xda;
 static const std::string Xing;
 static const std::string XingBox;
 static const std::string XingCircle;
 static const std::string Xml;
 static const std::string Yammer;
 static const std::string Yeast;
 static const std::string Yelp;
 static const std::string YinYang;
 static const std::string YoutubePlay;
 static const std::string ZipBox;

};

};
