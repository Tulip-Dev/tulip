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

To set a node glyph as a Material Design icon, the Material Design Icon glyph must be associated
to the node through the modification of the "viewShape" integer property attached to the graph.
The name of the icon to use must then be set in the "viewMaterialDesignIcon" string property.
As an example, the following code snippet activates the Font Awesome glyph for all nodes
and sets the "user" icon::

  # graph is an instance of a tlp.Graph object
  viewShape = graph.getIntegerProperty("viewShape")
  viewMaterialDesignIcon = graph.getStringProperty("viewMaterialDesignIcon")

  // sets the Material Design Icon glyph on all nodes
  viewShape.setAllNodeValue(tlp.NodeShape.MaterialDesignIcon)
  // sets the "user" glyph for all nodes
  viewMaterialDesignIconsIcon.setAllNodeValue(tlp.TulipMaterialDesignIcons.User);

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
  * :const:`tlp.TulipMaterialDesignIcons.AccountStarVariant` (:mdicon:`account-star-variant`)
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
  * :const:`tlp.TulipMaterialDesignIcons.AlertOctagon` (:mdicon:`alert-octagon`)
  * :const:`tlp.TulipMaterialDesignIcons.AlertOutline` (:mdicon:`alert-outline`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Appnet` (:mdicon:`appnet`)
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
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownBoldCircle` (:mdicon:`arrow-down-bold-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownBoldCircleOutline` (:mdicon:`arrow-down-bold-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownBoldHexagonOutline` (:mdicon:`arrow-down-bold-hexagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownDropCircle` (:mdicon:`arrow-down-drop-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowDownDropCircleOutline` (:mdicon:`arrow-down-drop-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowExpand` (:mdicon:`arrow-expand`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowExpandAll` (:mdicon:`arrow-expand-all`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeft` (:mdicon:`arrow-left`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBold` (:mdicon:`arrow-left-bold`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBoldCircle` (:mdicon:`arrow-left-bold-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBoldCircleOutline` (:mdicon:`arrow-left-bold-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftBoldHexagonOutline` (:mdicon:`arrow-left-bold-hexagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftDropCircle` (:mdicon:`arrow-left-drop-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowLeftDropCircleOutline` (:mdicon:`arrow-left-drop-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRight` (:mdicon:`arrow-right`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBold` (:mdicon:`arrow-right-bold`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBoldCircle` (:mdicon:`arrow-right-bold-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBoldCircleOutline` (:mdicon:`arrow-right-bold-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightBoldHexagonOutline` (:mdicon:`arrow-right-bold-hexagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightDropCircle` (:mdicon:`arrow-right-drop-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowRightDropCircleOutline` (:mdicon:`arrow-right-drop-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowTopLeft` (:mdicon:`arrow-top-left`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowTopRight` (:mdicon:`arrow-top-right`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUp` (:mdicon:`arrow-up`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBold` (:mdicon:`arrow-up-bold`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBoldCircle` (:mdicon:`arrow-up-bold-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBoldCircleOutline` (:mdicon:`arrow-up-bold-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpBoldHexagonOutline` (:mdicon:`arrow-up-bold-hexagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpDropCircle` (:mdicon:`arrow-up-drop-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.ArrowUpDropCircleOutline` (:mdicon:`arrow-up-drop-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Assistant` (:mdicon:`assistant`)
  * :const:`tlp.TulipMaterialDesignIcons.At` (:mdicon:`at`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Candle` (:mdicon:`candle`)
  * :const:`tlp.TulipMaterialDesignIcons.Candycane` (:mdicon:`candycane`)
  * :const:`tlp.TulipMaterialDesignIcons.Car` (:mdicon:`car`)
  * :const:`tlp.TulipMaterialDesignIcons.CarBattery` (:mdicon:`car-battery`)
  * :const:`tlp.TulipMaterialDesignIcons.CarConnected` (:mdicon:`car-connected`)
  * :const:`tlp.TulipMaterialDesignIcons.CarWash` (:mdicon:`car-wash`)
  * :const:`tlp.TulipMaterialDesignIcons.Cards` (:mdicon:`cards`)
  * :const:`tlp.TulipMaterialDesignIcons.CardsOutline` (:mdicon:`cards-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CardsPlayingOutline` (:mdicon:`cards-playing-outline`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Castle` (:mdicon:`castle`)
  * :const:`tlp.TulipMaterialDesignIcons.Cat` (:mdicon:`cat`)
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
  * :const:`tlp.TulipMaterialDesignIcons.ChartBubble` (:mdicon:`chart-bubble`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartGantt` (:mdicon:`chart-gantt`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartHistogram` (:mdicon:`chart-histogram`)
  * :const:`tlp.TulipMaterialDesignIcons.ChartLine` (:mdicon:`chart-line`)
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
  * :const:`tlp.TulipMaterialDesignIcons.CiscoWebex` (:mdicon:`cisco-webex`)
  * :const:`tlp.TulipMaterialDesignIcons.City` (:mdicon:`city`)
  * :const:`tlp.TulipMaterialDesignIcons.Clipboard` (:mdicon:`clipboard`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardAccount` (:mdicon:`clipboard-account`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardAlert` (:mdicon:`clipboard-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardArrowDown` (:mdicon:`clipboard-arrow-down`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardArrowLeft` (:mdicon:`clipboard-arrow-left`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardCheck` (:mdicon:`clipboard-check`)
  * :const:`tlp.TulipMaterialDesignIcons.ClipboardOutline` (:mdicon:`clipboard-outline`)
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
  * :const:`tlp.TulipMaterialDesignIcons.ClosedCaption` (:mdicon:`closed-caption`)
  * :const:`tlp.TulipMaterialDesignIcons.Cloud` (:mdicon:`cloud`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudCheck` (:mdicon:`cloud-check`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudCircle` (:mdicon:`cloud-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudDownload` (:mdicon:`cloud-download`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudOutline` (:mdicon:`cloud-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.CloudOutlineOff` (:mdicon:`cloud-outline-off`)
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
  * :const:`tlp.TulipMaterialDesignIcons.DecimalDecrease` (:mdicon:`decimal-decrease`)
  * :const:`tlp.TulipMaterialDesignIcons.DecimalIncrease` (:mdicon:`decimal-increase`)
  * :const:`tlp.TulipMaterialDesignIcons.Delete` (:mdicon:`delete`)
  * :const:`tlp.TulipMaterialDesignIcons.DeleteCircle` (:mdicon:`delete-circle`)
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
  * :const:`tlp.TulipMaterialDesignIcons.DiceD20` (:mdicon:`dice-d20`)
  * :const:`tlp.TulipMaterialDesignIcons.DiceD4` (:mdicon:`dice-d4`)
  * :const:`tlp.TulipMaterialDesignIcons.DiceD6` (:mdicon:`dice-d6`)
  * :const:`tlp.TulipMaterialDesignIcons.DiceD8` (:mdicon:`dice-d8`)
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
  * :const:`tlp.TulipMaterialDesignIcons.EarthOff` (:mdicon:`earth-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Edge` (:mdicon:`edge`)
  * :const:`tlp.TulipMaterialDesignIcons.Eject` (:mdicon:`eject`)
  * :const:`tlp.TulipMaterialDesignIcons.ElevationDecline` (:mdicon:`elevation-decline`)
  * :const:`tlp.TulipMaterialDesignIcons.ElevationRise` (:mdicon:`elevation-rise`)
  * :const:`tlp.TulipMaterialDesignIcons.Elevator` (:mdicon:`elevator`)
  * :const:`tlp.TulipMaterialDesignIcons.Email` (:mdicon:`email`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Fax` (:mdicon:`fax`)
  * :const:`tlp.TulipMaterialDesignIcons.Ferry` (:mdicon:`ferry`)
  * :const:`tlp.TulipMaterialDesignIcons.File` (:mdicon:`file`)
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
  * :const:`tlp.TulipMaterialDesignIcons.FolderOutline` (:mdicon:`folder-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderPlus` (:mdicon:`folder-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderRemove` (:mdicon:`folder-remove`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderStar` (:mdicon:`folder-star`)
  * :const:`tlp.TulipMaterialDesignIcons.FolderUpload` (:mdicon:`folder-upload`)
  * :const:`tlp.TulipMaterialDesignIcons.Food` (:mdicon:`food`)
  * :const:`tlp.TulipMaterialDesignIcons.FoodApple` (:mdicon:`food-apple`)
  * :const:`tlp.TulipMaterialDesignIcons.FoodForkDrink` (:mdicon:`food-fork-drink`)
  * :const:`tlp.TulipMaterialDesignIcons.FoodOff` (:mdicon:`food-off`)
  * :const:`tlp.TulipMaterialDesignIcons.FoodVariant` (:mdicon:`food-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Football` (:mdicon:`football`)
  * :const:`tlp.TulipMaterialDesignIcons.FootballAustralian` (:mdicon:`football-australian`)
  * :const:`tlp.TulipMaterialDesignIcons.FootballHelmet` (:mdicon:`football-helmet`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignCenter` (:mdicon:`format-align-center`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignJustify` (:mdicon:`format-align-justify`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignLeft` (:mdicon:`format-align-left`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAlignRight` (:mdicon:`format-align-right`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatAnnotationPlus` (:mdicon:`format-annotation-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatBold` (:mdicon:`format-bold`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatClear` (:mdicon:`format-clear`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatColorFill` (:mdicon:`format-color-fill`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatColorText` (:mdicon:`format-color-text`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatFloatCenter` (:mdicon:`format-float-center`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatFloatLeft` (:mdicon:`format-float-left`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatFloatNone` (:mdicon:`format-float-none`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatFloatRight` (:mdicon:`format-float-right`)
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
  * :const:`tlp.TulipMaterialDesignIcons.FormatListNumbers` (:mdicon:`format-list-numbers`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatPaint` (:mdicon:`format-paint`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatParagraph` (:mdicon:`format-paragraph`)
  * :const:`tlp.TulipMaterialDesignIcons.FormatQuote` (:mdicon:`format-quote`)
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
  * :const:`tlp.TulipMaterialDesignIcons.GasCylinder` (:mdicon:`gas-cylinder`)
  * :const:`tlp.TulipMaterialDesignIcons.GasStation` (:mdicon:`gas-station`)
  * :const:`tlp.TulipMaterialDesignIcons.Gate` (:mdicon:`gate`)
  * :const:`tlp.TulipMaterialDesignIcons.Gauge` (:mdicon:`gauge`)
  * :const:`tlp.TulipMaterialDesignIcons.Gavel` (:mdicon:`gavel`)
  * :const:`tlp.TulipMaterialDesignIcons.GenderFemale` (:mdicon:`gender-female`)
  * :const:`tlp.TulipMaterialDesignIcons.GenderMale` (:mdicon:`gender-male`)
  * :const:`tlp.TulipMaterialDesignIcons.GenderMaleFemale` (:mdicon:`gender-male-female`)
  * :const:`tlp.TulipMaterialDesignIcons.GenderTransgender` (:mdicon:`gender-transgender`)
  * :const:`tlp.TulipMaterialDesignIcons.Ghost` (:mdicon:`ghost`)
  * :const:`tlp.TulipMaterialDesignIcons.Gift` (:mdicon:`gift`)
  * :const:`tlp.TulipMaterialDesignIcons.Git` (:mdicon:`git`)
  * :const:`tlp.TulipMaterialDesignIcons.GithubBox` (:mdicon:`github-box`)
  * :const:`tlp.TulipMaterialDesignIcons.GithubCircle` (:mdicon:`github-circle`)
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
  * :const:`tlp.TulipMaterialDesignIcons.GoogleMaps` (:mdicon:`google-maps`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleNearby` (:mdicon:`google-nearby`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePages` (:mdicon:`google-pages`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePhysicalWeb` (:mdicon:`google-physical-web`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePlay` (:mdicon:`google-play`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePlus` (:mdicon:`google-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.GooglePlusBox` (:mdicon:`google-plus-box`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleTranslate` (:mdicon:`google-translate`)
  * :const:`tlp.TulipMaterialDesignIcons.GoogleWallet` (:mdicon:`google-wallet`)
  * :const:`tlp.TulipMaterialDesignIcons.Gradient` (:mdicon:`gradient`)
  * :const:`tlp.TulipMaterialDesignIcons.GreasePencil` (:mdicon:`grease-pencil`)
  * :const:`tlp.TulipMaterialDesignIcons.Grid` (:mdicon:`grid`)
  * :const:`tlp.TulipMaterialDesignIcons.GridOff` (:mdicon:`grid-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Group` (:mdicon:`group`)
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
  * :const:`tlp.TulipMaterialDesignIcons.HeartOutline` (:mdicon:`heart-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.HeartPulse` (:mdicon:`heart-pulse`)
  * :const:`tlp.TulipMaterialDesignIcons.Help` (:mdicon:`help`)
  * :const:`tlp.TulipMaterialDesignIcons.HelpCircle` (:mdicon:`help-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.HelpCircleOutline` (:mdicon:`help-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Hexagon` (:mdicon:`hexagon`)
  * :const:`tlp.TulipMaterialDesignIcons.HexagonOutline` (:mdicon:`hexagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Highway` (:mdicon:`highway`)
  * :const:`tlp.TulipMaterialDesignIcons.History` (:mdicon:`history`)
  * :const:`tlp.TulipMaterialDesignIcons.Hololens` (:mdicon:`hololens`)
  * :const:`tlp.TulipMaterialDesignIcons.Home` (:mdicon:`home`)
  * :const:`tlp.TulipMaterialDesignIcons.HomeMapMarker` (:mdicon:`home-map-marker`)
  * :const:`tlp.TulipMaterialDesignIcons.HomeModern` (:mdicon:`home-modern`)
  * :const:`tlp.TulipMaterialDesignIcons.HomeOutline` (:mdicon:`home-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.HomeVariant` (:mdicon:`home-variant`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Laptop` (:mdicon:`laptop`)
  * :const:`tlp.TulipMaterialDesignIcons.LaptopChromebook` (:mdicon:`laptop-chromebook`)
  * :const:`tlp.TulipMaterialDesignIcons.LaptopMac` (:mdicon:`laptop-mac`)
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
  * :const:`tlp.TulipMaterialDesignIcons.LightbulbOutline` (:mdicon:`lightbulb-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Link` (:mdicon:`link`)
  * :const:`tlp.TulipMaterialDesignIcons.LinkOff` (:mdicon:`link-off`)
  * :const:`tlp.TulipMaterialDesignIcons.LinkVariant` (:mdicon:`link-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.LinkVariantOff` (:mdicon:`link-variant-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Linkedin` (:mdicon:`linkedin`)
  * :const:`tlp.TulipMaterialDesignIcons.LinkedinBox` (:mdicon:`linkedin-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Linux` (:mdicon:`linux`)
  * :const:`tlp.TulipMaterialDesignIcons.Lock` (:mdicon:`lock`)
  * :const:`tlp.TulipMaterialDesignIcons.LockOpen` (:mdicon:`lock-open`)
  * :const:`tlp.TulipMaterialDesignIcons.LockOpenOutline` (:mdicon:`lock-open-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.LockOutline` (:mdicon:`lock-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.LockPlus` (:mdicon:`lock-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.Login` (:mdicon:`login`)
  * :const:`tlp.TulipMaterialDesignIcons.LoginVariant` (:mdicon:`login-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Logout` (:mdicon:`logout`)
  * :const:`tlp.TulipMaterialDesignIcons.LogoutVariant` (:mdicon:`logout-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Looks` (:mdicon:`looks`)
  * :const:`tlp.TulipMaterialDesignIcons.Loupe` (:mdicon:`loupe`)
  * :const:`tlp.TulipMaterialDesignIcons.Lumx` (:mdicon:`lumx`)
  * :const:`tlp.TulipMaterialDesignIcons.Magnet` (:mdicon:`magnet`)
  * :const:`tlp.TulipMaterialDesignIcons.MagnetOn` (:mdicon:`magnet-on`)
  * :const:`tlp.TulipMaterialDesignIcons.Magnify` (:mdicon:`magnify`)
  * :const:`tlp.TulipMaterialDesignIcons.MagnifyMinus` (:mdicon:`magnify-minus`)
  * :const:`tlp.TulipMaterialDesignIcons.MagnifyPlus` (:mdicon:`magnify-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.MailRu` (:mdicon:`mail-ru`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Nature` (:mdicon:`nature`)
  * :const:`tlp.TulipMaterialDesignIcons.NaturePeople` (:mdicon:`nature-people`)
  * :const:`tlp.TulipMaterialDesignIcons.Navigation` (:mdicon:`navigation`)
  * :const:`tlp.TulipMaterialDesignIcons.NearMe` (:mdicon:`near-me`)
  * :const:`tlp.TulipMaterialDesignIcons.Needle` (:mdicon:`needle`)
  * :const:`tlp.TulipMaterialDesignIcons.NestProtect` (:mdicon:`nest-protect`)
  * :const:`tlp.TulipMaterialDesignIcons.NestThermostat` (:mdicon:`nest-thermostat`)
  * :const:`tlp.TulipMaterialDesignIcons.NewBox` (:mdicon:`new-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Newspaper` (:mdicon:`newspaper`)
  * :const:`tlp.TulipMaterialDesignIcons.Nfc` (:mdicon:`nfc`)
  * :const:`tlp.TulipMaterialDesignIcons.NfcTap` (:mdicon:`nfc-tap`)
  * :const:`tlp.TulipMaterialDesignIcons.NfcVariant` (:mdicon:`nfc-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Nodejs` (:mdicon:`nodejs`)
  * :const:`tlp.TulipMaterialDesignIcons.Note` (:mdicon:`note`)
  * :const:`tlp.TulipMaterialDesignIcons.NoteMultiple` (:mdicon:`note-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.NoteMultipleOutline` (:mdicon:`note-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.NoteOutline` (:mdicon:`note-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.NotePlus` (:mdicon:`note-plus`)
  * :const:`tlp.TulipMaterialDesignIcons.NotePlusOutline` (:mdicon:`note-plus-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.NoteText` (:mdicon:`note-text`)
  * :const:`tlp.TulipMaterialDesignIcons.NotificationClearAll` (:mdicon:`notification-clear-all`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Nutrition` (:mdicon:`nutrition`)
  * :const:`tlp.TulipMaterialDesignIcons.Oar` (:mdicon:`oar`)
  * :const:`tlp.TulipMaterialDesignIcons.Octagon` (:mdicon:`octagon`)
  * :const:`tlp.TulipMaterialDesignIcons.OctagonOutline` (:mdicon:`octagon-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Odnoklassniki` (:mdicon:`odnoklassniki`)
  * :const:`tlp.TulipMaterialDesignIcons.Office` (:mdicon:`office`)
  * :const:`tlp.TulipMaterialDesignIcons.Oil` (:mdicon:`oil`)
  * :const:`tlp.TulipMaterialDesignIcons.OilTemperature` (:mdicon:`oil-temperature`)
  * :const:`tlp.TulipMaterialDesignIcons.Omega` (:mdicon:`omega`)
  * :const:`tlp.TulipMaterialDesignIcons.Onedrive` (:mdicon:`onedrive`)
  * :const:`tlp.TulipMaterialDesignIcons.Opacity` (:mdicon:`opacity`)
  * :const:`tlp.TulipMaterialDesignIcons.OpenInApp` (:mdicon:`open-in-app`)
  * :const:`tlp.TulipMaterialDesignIcons.OpenInNew` (:mdicon:`open-in-new`)
  * :const:`tlp.TulipMaterialDesignIcons.Openid` (:mdicon:`openid`)
  * :const:`tlp.TulipMaterialDesignIcons.Opera` (:mdicon:`opera`)
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
  * :const:`tlp.TulipMaterialDesignIcons.PencilLock` (:mdicon:`pencil-lock`)
  * :const:`tlp.TulipMaterialDesignIcons.PencilOff` (:mdicon:`pencil-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Percent` (:mdicon:`percent`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Pin` (:mdicon:`pin`)
  * :const:`tlp.TulipMaterialDesignIcons.PinOff` (:mdicon:`pin-off`)
  * :const:`tlp.TulipMaterialDesignIcons.PineTree` (:mdicon:`pine-tree`)
  * :const:`tlp.TulipMaterialDesignIcons.PineTreeBox` (:mdicon:`pine-tree-box`)
  * :const:`tlp.TulipMaterialDesignIcons.Pinterest` (:mdicon:`pinterest`)
  * :const:`tlp.TulipMaterialDesignIcons.PinterestBox` (:mdicon:`pinterest-box`)
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
  * :const:`tlp.TulipMaterialDesignIcons.PlusCircle` (:mdicon:`plus-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusCircleMultipleOutline` (:mdicon:`plus-circle-multiple-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusCircleOutline` (:mdicon:`plus-circle-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusNetwork` (:mdicon:`plus-network`)
  * :const:`tlp.TulipMaterialDesignIcons.PlusOne` (:mdicon:`plus-one`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Presentation` (:mdicon:`presentation`)
  * :const:`tlp.TulipMaterialDesignIcons.PresentationPlay` (:mdicon:`presentation-play`)
  * :const:`tlp.TulipMaterialDesignIcons.Printer` (:mdicon:`printer`)
  * :const:`tlp.TulipMaterialDesignIcons.Printer3d` (:mdicon:`printer-3d`)
  * :const:`tlp.TulipMaterialDesignIcons.PrinterAlert` (:mdicon:`printer-alert`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Restore` (:mdicon:`restore`)
  * :const:`tlp.TulipMaterialDesignIcons.Rewind` (:mdicon:`rewind`)
  * :const:`tlp.TulipMaterialDesignIcons.Ribbon` (:mdicon:`ribbon`)
  * :const:`tlp.TulipMaterialDesignIcons.Road` (:mdicon:`road`)
  * :const:`tlp.TulipMaterialDesignIcons.RoadVariant` (:mdicon:`road-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Robot` (:mdicon:`robot`)
  * :const:`tlp.TulipMaterialDesignIcons.Rocket` (:mdicon:`rocket`)
  * :const:`tlp.TulipMaterialDesignIcons.Rotate3d` (:mdicon:`rotate-3d`)
  * :const:`tlp.TulipMaterialDesignIcons.Rotate90` (:mdicon:`rotate-90`)
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
  * :const:`tlp.TulipMaterialDesignIcons.ShieldOutline` (:mdicon:`shield-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Shopping` (:mdicon:`shopping`)
  * :const:`tlp.TulipMaterialDesignIcons.ShoppingMusic` (:mdicon:`shopping-music`)
  * :const:`tlp.TulipMaterialDesignIcons.Shredder` (:mdicon:`shredder`)
  * :const:`tlp.TulipMaterialDesignIcons.Shuffle` (:mdicon:`shuffle`)
  * :const:`tlp.TulipMaterialDesignIcons.ShuffleDisabled` (:mdicon:`shuffle-disabled`)
  * :const:`tlp.TulipMaterialDesignIcons.ShuffleVariant` (:mdicon:`shuffle-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.Sigma` (:mdicon:`sigma`)
  * :const:`tlp.TulipMaterialDesignIcons.SigmaLower` (:mdicon:`sigma-lower`)
  * :const:`tlp.TulipMaterialDesignIcons.SignCaution` (:mdicon:`sign-caution`)
  * :const:`tlp.TulipMaterialDesignIcons.Signal` (:mdicon:`signal`)
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
  * :const:`tlp.TulipMaterialDesignIcons.SourceFork` (:mdicon:`source-fork`)
  * :const:`tlp.TulipMaterialDesignIcons.SourceMerge` (:mdicon:`source-merge`)
  * :const:`tlp.TulipMaterialDesignIcons.SourcePull` (:mdicon:`source-pull`)
  * :const:`tlp.TulipMaterialDesignIcons.Speaker` (:mdicon:`speaker`)
  * :const:`tlp.TulipMaterialDesignIcons.SpeakerOff` (:mdicon:`speaker-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Speedometer` (:mdicon:`speedometer`)
  * :const:`tlp.TulipMaterialDesignIcons.Spellcheck` (:mdicon:`spellcheck`)
  * :const:`tlp.TulipMaterialDesignIcons.Spotify` (:mdicon:`spotify`)
  * :const:`tlp.TulipMaterialDesignIcons.Spotlight` (:mdicon:`spotlight`)
  * :const:`tlp.TulipMaterialDesignIcons.SpotlightBeam` (:mdicon:`spotlight-beam`)
  * :const:`tlp.TulipMaterialDesignIcons.Spray` (:mdicon:`spray`)
  * :const:`tlp.TulipMaterialDesignIcons.SquareInc` (:mdicon:`square-inc`)
  * :const:`tlp.TulipMaterialDesignIcons.SquareIncCash` (:mdicon:`square-inc-cash`)
  * :const:`tlp.TulipMaterialDesignIcons.Stackexchange` (:mdicon:`stackexchange`)
  * :const:`tlp.TulipMaterialDesignIcons.Stackoverflow` (:mdicon:`stackoverflow`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Sunglasses` (:mdicon:`sunglasses`)
  * :const:`tlp.TulipMaterialDesignIcons.SurroundSound` (:mdicon:`surround-sound`)
  * :const:`tlp.TulipMaterialDesignIcons.SwapHorizontal` (:mdicon:`swap-horizontal`)
  * :const:`tlp.TulipMaterialDesignIcons.SwapVertical` (:mdicon:`swap-vertical`)
  * :const:`tlp.TulipMaterialDesignIcons.Swim` (:mdicon:`swim`)
  * :const:`tlp.TulipMaterialDesignIcons.Switch` (:mdicon:`switch`)
  * :const:`tlp.TulipMaterialDesignIcons.Sword` (:mdicon:`sword`)
  * :const:`tlp.TulipMaterialDesignIcons.Sync` (:mdicon:`sync`)
  * :const:`tlp.TulipMaterialDesignIcons.SyncAlert` (:mdicon:`sync-alert`)
  * :const:`tlp.TulipMaterialDesignIcons.SyncOff` (:mdicon:`sync-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Tab` (:mdicon:`tab`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Tag` (:mdicon:`tag`)
  * :const:`tlp.TulipMaterialDesignIcons.TagFaces` (:mdicon:`tag-faces`)
  * :const:`tlp.TulipMaterialDesignIcons.TagHeart` (:mdicon:`tag-heart`)
  * :const:`tlp.TulipMaterialDesignIcons.TagMultiple` (:mdicon:`tag-multiple`)
  * :const:`tlp.TulipMaterialDesignIcons.TagOutline` (:mdicon:`tag-outline`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Tie` (:mdicon:`tie`)
  * :const:`tlp.TulipMaterialDesignIcons.Timelapse` (:mdicon:`timelapse`)
  * :const:`tlp.TulipMaterialDesignIcons.Timer` (:mdicon:`timer`)
  * :const:`tlp.TulipMaterialDesignIcons.Timer10` (:mdicon:`timer-10`)
  * :const:`tlp.TulipMaterialDesignIcons.Timer3` (:mdicon:`timer-3`)
  * :const:`tlp.TulipMaterialDesignIcons.TimerOff` (:mdicon:`timer-off`)
  * :const:`tlp.TulipMaterialDesignIcons.TimerSand` (:mdicon:`timer-sand`)
  * :const:`tlp.TulipMaterialDesignIcons.TimerSandEmpty` (:mdicon:`timer-sand-empty`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Ubuntu` (:mdicon:`ubuntu`)
  * :const:`tlp.TulipMaterialDesignIcons.Umbraco` (:mdicon:`umbraco`)
  * :const:`tlp.TulipMaterialDesignIcons.Umbrella` (:mdicon:`umbrella`)
  * :const:`tlp.TulipMaterialDesignIcons.UmbrellaOutline` (:mdicon:`umbrella-outline`)
  * :const:`tlp.TulipMaterialDesignIcons.Undo` (:mdicon:`undo`)
  * :const:`tlp.TulipMaterialDesignIcons.UndoVariant` (:mdicon:`undo-variant`)
  * :const:`tlp.TulipMaterialDesignIcons.UnfoldLess` (:mdicon:`unfold-less`)
  * :const:`tlp.TulipMaterialDesignIcons.UnfoldMore` (:mdicon:`unfold-more`)
  * :const:`tlp.TulipMaterialDesignIcons.Ungroup` (:mdicon:`ungroup`)
  * :const:`tlp.TulipMaterialDesignIcons.Unity` (:mdicon:`unity`)
  * :const:`tlp.TulipMaterialDesignIcons.Untappd` (:mdicon:`untappd`)
  * :const:`tlp.TulipMaterialDesignIcons.Update` (:mdicon:`update`)
  * :const:`tlp.TulipMaterialDesignIcons.Upload` (:mdicon:`upload`)
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
  * :const:`tlp.TulipMaterialDesignIcons.ViewQuilt` (:mdicon:`view-quilt`)
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
  * :const:`tlp.TulipMaterialDesignIcons.VolumeOff` (:mdicon:`volume-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Vpn` (:mdicon:`vpn`)
  * :const:`tlp.TulipMaterialDesignIcons.Walk` (:mdicon:`walk`)
  * :const:`tlp.TulipMaterialDesignIcons.Wallet` (:mdicon:`wallet`)
  * :const:`tlp.TulipMaterialDesignIcons.WalletGiftcard` (:mdicon:`wallet-giftcard`)
  * :const:`tlp.TulipMaterialDesignIcons.WalletMembership` (:mdicon:`wallet-membership`)
  * :const:`tlp.TulipMaterialDesignIcons.WalletTravel` (:mdicon:`wallet-travel`)
  * :const:`tlp.TulipMaterialDesignIcons.Wan` (:mdicon:`wan`)
  * :const:`tlp.TulipMaterialDesignIcons.Watch` (:mdicon:`watch`)
  * :const:`tlp.TulipMaterialDesignIcons.WatchExport` (:mdicon:`watch-export`)
  * :const:`tlp.TulipMaterialDesignIcons.WatchImport` (:mdicon:`watch-import`)
  * :const:`tlp.TulipMaterialDesignIcons.WatchVibrate` (:mdicon:`watch-vibrate`)
  * :const:`tlp.TulipMaterialDesignIcons.Water` (:mdicon:`water`)
  * :const:`tlp.TulipMaterialDesignIcons.WaterOff` (:mdicon:`water-off`)
  * :const:`tlp.TulipMaterialDesignIcons.WaterPercent` (:mdicon:`water-percent`)
  * :const:`tlp.TulipMaterialDesignIcons.WaterPump` (:mdicon:`water-pump`)
  * :const:`tlp.TulipMaterialDesignIcons.Watermark` (:mdicon:`watermark`)
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
  * :const:`tlp.TulipMaterialDesignIcons.Wechat` (:mdicon:`wechat`)
  * :const:`tlp.TulipMaterialDesignIcons.Weight` (:mdicon:`weight`)
  * :const:`tlp.TulipMaterialDesignIcons.WeightKilogram` (:mdicon:`weight-kilogram`)
  * :const:`tlp.TulipMaterialDesignIcons.Whatsapp` (:mdicon:`whatsapp`)
  * :const:`tlp.TulipMaterialDesignIcons.WheelchairAccessibility` (:mdicon:`wheelchair-accessibility`)
  * :const:`tlp.TulipMaterialDesignIcons.WhiteBalanceAuto` (:mdicon:`white-balance-auto`)
  * :const:`tlp.TulipMaterialDesignIcons.WhiteBalanceIncandescent` (:mdicon:`white-balance-incandescent`)
  * :const:`tlp.TulipMaterialDesignIcons.WhiteBalanceIridescent` (:mdicon:`white-balance-iridescent`)
  * :const:`tlp.TulipMaterialDesignIcons.WhiteBalanceSunny` (:mdicon:`white-balance-sunny`)
  * :const:`tlp.TulipMaterialDesignIcons.Wifi` (:mdicon:`wifi`)
  * :const:`tlp.TulipMaterialDesignIcons.WifiOff` (:mdicon:`wifi-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Wii` (:mdicon:`wii`)
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
  * :const:`tlp.TulipMaterialDesignIcons.XboxControllerOff` (:mdicon:`xbox-controller-off`)
  * :const:`tlp.TulipMaterialDesignIcons.Xda` (:mdicon:`xda`)
  * :const:`tlp.TulipMaterialDesignIcons.Xing` (:mdicon:`xing`)
  * :const:`tlp.TulipMaterialDesignIcons.XingBox` (:mdicon:`xing-box`)
  * :const:`tlp.TulipMaterialDesignIcons.XingCircle` (:mdicon:`xing-circle`)
  * :const:`tlp.TulipMaterialDesignIcons.Xml` (:mdicon:`xml`)
  * :const:`tlp.TulipMaterialDesignIcons.Yeast` (:mdicon:`yeast`)
  * :const:`tlp.TulipMaterialDesignIcons.Yelp` (:mdicon:`yelp`)
  * :const:`tlp.TulipMaterialDesignIcons.YinYang` (:mdicon:`yin-yang`)
  * :const:`tlp.TulipMaterialDesignIcons.YoutubePlay` (:mdicon:`youtube-play`)
  * :const:`tlp.TulipMaterialDesignIcons.ZipBox` (:mdicon:`zip-box`)

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

  static const char* AccessPoint;
  static const char* AccessPointNetwork;
  static const char* Account;
  static const char* AccountAlert;
  static const char* AccountBox;
  static const char* AccountBoxOutline;
  static const char* AccountCardDetails;
  static const char* AccountCheck;
  static const char* AccountCircle;
  static const char* AccountConvert;
  static const char* AccountKey;
  static const char* AccountLocation;
  static const char* AccountMinus;
  static const char* AccountMultiple;
  static const char* AccountMultipleMinus;
  static const char* AccountMultipleOutline;
  static const char* AccountMultiplePlus;
  static const char* AccountNetwork;
  static const char* AccountOff;
  static const char* AccountOutline;
  static const char* AccountPlus;
  static const char* AccountRemove;
  static const char* AccountSearch;
  static const char* AccountSettings;
  static const char* AccountSettingsVariant;
  static const char* AccountStar;
  static const char* AccountStarVariant;
  static const char* AccountSwitch;
  static const char* Adjust;
  static const char* AirConditioner;
  static const char* Airballoon;
  static const char* Airplane;
  static const char* AirplaneLanding;
  static const char* AirplaneOff;
  static const char* AirplaneTakeoff;
  static const char* Airplay;
  static const char* Alarm;
  static const char* AlarmCheck;
  static const char* AlarmMultiple;
  static const char* AlarmOff;
  static const char* AlarmPlus;
  static const char* AlarmSnooze;
  static const char* Album;
  static const char* Alert;
  static const char* AlertBox;
  static const char* AlertCircle;
  static const char* AlertCircleOutline;
  static const char* AlertOctagon;
  static const char* AlertOutline;
  static const char* Alpha;
  static const char* Alphabetical;
  static const char* Altimeter;
  static const char* Amazon;
  static const char* AmazonClouddrive;
  static const char* Ambulance;
  static const char* Amplifier;
  static const char* Anchor;
  static const char* Android;
  static const char* AndroidDebugBridge;
  static const char* AndroidStudio;
  static const char* Angular;
  static const char* Animation;
  static const char* Apple;
  static const char* AppleFinder;
  static const char* AppleIos;
  static const char* AppleKeyboardCaps;
  static const char* AppleKeyboardCommand;
  static const char* AppleKeyboardControl;
  static const char* AppleKeyboardOption;
  static const char* AppleKeyboardShift;
  static const char* AppleMobileme;
  static const char* AppleSafari;
  static const char* Application;
  static const char* Appnet;
  static const char* Apps;
  static const char* Archive;
  static const char* ArrangeBringForward;
  static const char* ArrangeBringToFront;
  static const char* ArrangeSendBackward;
  static const char* ArrangeSendToBack;
  static const char* ArrowAll;
  static const char* ArrowBottomLeft;
  static const char* ArrowBottomRight;
  static const char* ArrowCompress;
  static const char* ArrowCompressAll;
  static const char* ArrowDown;
  static const char* ArrowDownBold;
  static const char* ArrowDownBoldCircle;
  static const char* ArrowDownBoldCircleOutline;
  static const char* ArrowDownBoldHexagonOutline;
  static const char* ArrowDownDropCircle;
  static const char* ArrowDownDropCircleOutline;
  static const char* ArrowExpand;
  static const char* ArrowExpandAll;
  static const char* ArrowLeft;
  static const char* ArrowLeftBold;
  static const char* ArrowLeftBoldCircle;
  static const char* ArrowLeftBoldCircleOutline;
  static const char* ArrowLeftBoldHexagonOutline;
  static const char* ArrowLeftDropCircle;
  static const char* ArrowLeftDropCircleOutline;
  static const char* ArrowRight;
  static const char* ArrowRightBold;
  static const char* ArrowRightBoldCircle;
  static const char* ArrowRightBoldCircleOutline;
  static const char* ArrowRightBoldHexagonOutline;
  static const char* ArrowRightDropCircle;
  static const char* ArrowRightDropCircleOutline;
  static const char* ArrowTopLeft;
  static const char* ArrowTopRight;
  static const char* ArrowUp;
  static const char* ArrowUpBold;
  static const char* ArrowUpBoldCircle;
  static const char* ArrowUpBoldCircleOutline;
  static const char* ArrowUpBoldHexagonOutline;
  static const char* ArrowUpDropCircle;
  static const char* ArrowUpDropCircleOutline;
  static const char* Assistant;
  static const char* At;
  static const char* Attachment;
  static const char* Audiobook;
  static const char* AutoFix;
  static const char* AutoUpload;
  static const char* Autorenew;
  static const char* AvTimer;
  static const char* Baby;
  static const char* BabyBuggy;
  static const char* Backburger;
  static const char* Backspace;
  static const char* BackupRestore;
  static const char* Bandcamp;
  static const char* Bank;
  static const char* Barcode;
  static const char* BarcodeScan;
  static const char* Barley;
  static const char* Barrel;
  static const char* Basecamp;
  static const char* Basket;
  static const char* BasketFill;
  static const char* BasketUnfill;
  static const char* Battery;
  static const char* Battery10;
  static const char* Battery20;
  static const char* Battery30;
  static const char* Battery40;
  static const char* Battery50;
  static const char* Battery60;
  static const char* Battery70;
  static const char* Battery80;
  static const char* Battery90;
  static const char* BatteryAlert;
  static const char* BatteryCharging;
  static const char* BatteryCharging100;
  static const char* BatteryCharging20;
  static const char* BatteryCharging30;
  static const char* BatteryCharging40;
  static const char* BatteryCharging60;
  static const char* BatteryCharging80;
  static const char* BatteryCharging90;
  static const char* BatteryMinus;
  static const char* BatteryNegative;
  static const char* BatteryOutline;
  static const char* BatteryPlus;
  static const char* BatteryPositive;
  static const char* BatteryUnknown;
  static const char* Beach;
  static const char* Beaker;
  static const char* Beats;
  static const char* Beer;
  static const char* Behance;
  static const char* Bell;
  static const char* BellOff;
  static const char* BellOutline;
  static const char* BellPlus;
  static const char* BellRing;
  static const char* BellRingOutline;
  static const char* BellSleep;
  static const char* Beta;
  static const char* Bible;
  static const char* Bike;
  static const char* Bing;
  static const char* Binoculars;
  static const char* Bio;
  static const char* Biohazard;
  static const char* Bitbucket;
  static const char* BlackMesa;
  static const char* Blackberry;
  static const char* Blender;
  static const char* Blinds;
  static const char* BlockHelper;
  static const char* Blogger;
  static const char* Bluetooth;
  static const char* BluetoothAudio;
  static const char* BluetoothConnect;
  static const char* BluetoothOff;
  static const char* BluetoothSettings;
  static const char* BluetoothTransfer;
  static const char* Blur;
  static const char* BlurLinear;
  static const char* BlurOff;
  static const char* BlurRadial;
  static const char* Bomb;
  static const char* Bone;
  static const char* Book;
  static const char* BookMinus;
  static const char* BookMultiple;
  static const char* BookMultipleVariant;
  static const char* BookOpen;
  static const char* BookOpenPageVariant;
  static const char* BookOpenVariant;
  static const char* BookPlus;
  static const char* BookVariant;
  static const char* Bookmark;
  static const char* BookmarkCheck;
  static const char* BookmarkMusic;
  static const char* BookmarkOutline;
  static const char* BookmarkPlus;
  static const char* BookmarkPlusOutline;
  static const char* BookmarkRemove;
  static const char* Boombox;
  static const char* BorderAll;
  static const char* BorderBottom;
  static const char* BorderColor;
  static const char* BorderHorizontal;
  static const char* BorderInside;
  static const char* BorderLeft;
  static const char* BorderNone;
  static const char* BorderOutside;
  static const char* BorderRight;
  static const char* BorderStyle;
  static const char* BorderTop;
  static const char* BorderVertical;
  static const char* BowTie;
  static const char* Bowl;
  static const char* Bowling;
  static const char* Box;
  static const char* BoxCutter;
  static const char* BoxShadow;
  static const char* Bridge;
  static const char* Briefcase;
  static const char* BriefcaseCheck;
  static const char* BriefcaseDownload;
  static const char* BriefcaseUpload;
  static const char* Brightness1;
  static const char* Brightness2;
  static const char* Brightness3;
  static const char* Brightness4;
  static const char* Brightness5;
  static const char* Brightness6;
  static const char* Brightness7;
  static const char* BrightnessAuto;
  static const char* Broom;
  static const char* Brush;
  static const char* Buffer;
  static const char* Bug;
  static const char* BulletinBoard;
  static const char* Bullhorn;
  static const char* Bullseye;
  static const char* BurstMode;
  static const char* Bus;
  static const char* Cached;
  static const char* Cake;
  static const char* CakeLayered;
  static const char* CakeVariant;
  static const char* Calculator;
  static const char* Calendar;
  static const char* CalendarBlank;
  static const char* CalendarCheck;
  static const char* CalendarClock;
  static const char* CalendarMultiple;
  static const char* CalendarMultipleCheck;
  static const char* CalendarPlus;
  static const char* CalendarQuestion;
  static const char* CalendarRange;
  static const char* CalendarRemove;
  static const char* CalendarText;
  static const char* CalendarToday;
  static const char* CallMade;
  static const char* CallMerge;
  static const char* CallMissed;
  static const char* CallReceived;
  static const char* CallSplit;
  static const char* Camcorder;
  static const char* CamcorderBox;
  static const char* CamcorderBoxOff;
  static const char* CamcorderOff;
  static const char* Camera;
  static const char* CameraBurst;
  static const char* CameraEnhance;
  static const char* CameraFront;
  static const char* CameraFrontVariant;
  static const char* CameraIris;
  static const char* CameraOff;
  static const char* CameraPartyMode;
  static const char* CameraRear;
  static const char* CameraRearVariant;
  static const char* CameraSwitch;
  static const char* CameraTimer;
  static const char* Candle;
  static const char* Candycane;
  static const char* Car;
  static const char* CarBattery;
  static const char* CarConnected;
  static const char* CarWash;
  static const char* Cards;
  static const char* CardsOutline;
  static const char* CardsPlayingOutline;
  static const char* Carrot;
  static const char* Cart;
  static const char* CartOff;
  static const char* CartOutline;
  static const char* CartPlus;
  static const char* CaseSensitiveAlt;
  static const char* Cash;
  static const char* Cash100;
  static const char* CashMultiple;
  static const char* CashUsd;
  static const char* Cast;
  static const char* CastConnected;
  static const char* Castle;
  static const char* Cat;
  static const char* Cellphone;
  static const char* CellphoneAndroid;
  static const char* CellphoneBasic;
  static const char* CellphoneDock;
  static const char* CellphoneIphone;
  static const char* CellphoneLink;
  static const char* CellphoneLinkOff;
  static const char* CellphoneSettings;
  static const char* Certificate;
  static const char* ChairSchool;
  static const char* ChartArc;
  static const char* ChartAreaspline;
  static const char* ChartBar;
  static const char* ChartBubble;
  static const char* ChartGantt;
  static const char* ChartHistogram;
  static const char* ChartLine;
  static const char* ChartPie;
  static const char* ChartScatterplotHexbin;
  static const char* ChartTimeline;
  static const char* Check;
  static const char* CheckAll;
  static const char* CheckCircle;
  static const char* CheckCircleOutline;
  static const char* CheckboxBlank;
  static const char* CheckboxBlankCircle;
  static const char* CheckboxBlankCircleOutline;
  static const char* CheckboxBlankOutline;
  static const char* CheckboxMarked;
  static const char* CheckboxMarkedCircle;
  static const char* CheckboxMarkedCircleOutline;
  static const char* CheckboxMarkedOutline;
  static const char* CheckboxMultipleBlank;
  static const char* CheckboxMultipleBlankCircle;
  static const char* CheckboxMultipleBlankCircleOutline;
  static const char* CheckboxMultipleBlankOutline;
  static const char* CheckboxMultipleMarked;
  static const char* CheckboxMultipleMarkedCircle;
  static const char* CheckboxMultipleMarkedCircleOutline;
  static const char* CheckboxMultipleMarkedOutline;
  static const char* Checkerboard;
  static const char* ChemicalWeapon;
  static const char* ChevronDoubleDown;
  static const char* ChevronDoubleLeft;
  static const char* ChevronDoubleRight;
  static const char* ChevronDoubleUp;
  static const char* ChevronDown;
  static const char* ChevronLeft;
  static const char* ChevronRight;
  static const char* ChevronUp;
  static const char* Chip;
  static const char* Church;
  static const char* CiscoWebex;
  static const char* City;
  static const char* Clipboard;
  static const char* ClipboardAccount;
  static const char* ClipboardAlert;
  static const char* ClipboardArrowDown;
  static const char* ClipboardArrowLeft;
  static const char* ClipboardCheck;
  static const char* ClipboardOutline;
  static const char* ClipboardText;
  static const char* Clippy;
  static const char* Clock;
  static const char* ClockAlert;
  static const char* ClockEnd;
  static const char* ClockFast;
  static const char* ClockIn;
  static const char* ClockOut;
  static const char* ClockStart;
  static const char* Close;
  static const char* CloseBox;
  static const char* CloseBoxOutline;
  static const char* CloseCircle;
  static const char* CloseCircleOutline;
  static const char* CloseNetwork;
  static const char* CloseOctagon;
  static const char* CloseOctagonOutline;
  static const char* ClosedCaption;
  static const char* Cloud;
  static const char* CloudCheck;
  static const char* CloudCircle;
  static const char* CloudDownload;
  static const char* CloudOutline;
  static const char* CloudOutlineOff;
  static const char* CloudPrint;
  static const char* CloudPrintOutline;
  static const char* CloudSync;
  static const char* CloudUpload;
  static const char* CodeArray;
  static const char* CodeBraces;
  static const char* CodeBrackets;
  static const char* CodeEqual;
  static const char* CodeGreaterThan;
  static const char* CodeGreaterThanOrEqual;
  static const char* CodeLessThan;
  static const char* CodeLessThanOrEqual;
  static const char* CodeNotEqual;
  static const char* CodeNotEqualVariant;
  static const char* CodeParentheses;
  static const char* CodeString;
  static const char* CodeTags;
  static const char* CodeTagsCheck;
  static const char* Codepen;
  static const char* Coffee;
  static const char* CoffeeToGo;
  static const char* Coin;
  static const char* Coins;
  static const char* Collage;
  static const char* ColorHelper;
  static const char* Comment;
  static const char* CommentAccount;
  static const char* CommentAccountOutline;
  static const char* CommentAlert;
  static const char* CommentAlertOutline;
  static const char* CommentCheck;
  static const char* CommentCheckOutline;
  static const char* CommentMultipleOutline;
  static const char* CommentOutline;
  static const char* CommentPlusOutline;
  static const char* CommentProcessing;
  static const char* CommentProcessingOutline;
  static const char* CommentQuestionOutline;
  static const char* CommentRemoveOutline;
  static const char* CommentText;
  static const char* CommentTextOutline;
  static const char* Compare;
  static const char* Compass;
  static const char* CompassOutline;
  static const char* Console;
  static const char* ContactMail;
  static const char* ContentCopy;
  static const char* ContentCut;
  static const char* ContentDuplicate;
  static const char* ContentPaste;
  static const char* ContentSave;
  static const char* ContentSaveAll;
  static const char* ContentSaveSettings;
  static const char* Contrast;
  static const char* ContrastBox;
  static const char* ContrastCircle;
  static const char* Cookie;
  static const char* Copyright;
  static const char* Counter;
  static const char* Cow;
  static const char* Creation;
  static const char* CreditCard;
  static const char* CreditCardMultiple;
  static const char* CreditCardOff;
  static const char* CreditCardPlus;
  static const char* CreditCardScan;
  static const char* Crop;
  static const char* CropFree;
  static const char* CropLandscape;
  static const char* CropPortrait;
  static const char* CropRotate;
  static const char* CropSquare;
  static const char* Crosshairs;
  static const char* CrosshairsGps;
  static const char* Crown;
  static const char* Cube;
  static const char* CubeOutline;
  static const char* CubeSend;
  static const char* CubeUnfolded;
  static const char* Cup;
  static const char* CupOff;
  static const char* CupWater;
  static const char* CurrencyBtc;
  static const char* CurrencyEur;
  static const char* CurrencyGbp;
  static const char* CurrencyInr;
  static const char* CurrencyNgn;
  static const char* CurrencyRub;
  static const char* CurrencyTry;
  static const char* CurrencyUsd;
  static const char* CurrencyUsdOff;
  static const char* CursorDefault;
  static const char* CursorDefaultOutline;
  static const char* CursorMove;
  static const char* CursorPointer;
  static const char* CursorText;
  static const char* Database;
  static const char* DatabaseMinus;
  static const char* DatabasePlus;
  static const char* DebugStepInto;
  static const char* DebugStepOut;
  static const char* DebugStepOver;
  static const char* DecimalDecrease;
  static const char* DecimalIncrease;
  static const char* Delete;
  static const char* DeleteCircle;
  static const char* DeleteForever;
  static const char* DeleteSweep;
  static const char* DeleteVariant;
  static const char* Delta;
  static const char* Deskphone;
  static const char* DesktopMac;
  static const char* DesktopTower;
  static const char* Details;
  static const char* DeveloperBoard;
  static const char* Deviantart;
  static const char* Dialpad;
  static const char* Diamond;
  static const char* Dice1;
  static const char* Dice2;
  static const char* Dice3;
  static const char* Dice4;
  static const char* Dice5;
  static const char* Dice6;
  static const char* DiceD20;
  static const char* DiceD4;
  static const char* DiceD6;
  static const char* DiceD8;
  static const char* Dictionary;
  static const char* Directions;
  static const char* DirectionsFork;
  static const char* Discord;
  static const char* Disk;
  static const char* DiskAlert;
  static const char* Disqus;
  static const char* DisqusOutline;
  static const char* Division;
  static const char* DivisionBox;
  static const char* Dna;
  static const char* Dns;
  static const char* DoNotDisturb;
  static const char* DoNotDisturbOff;
  static const char* Dolby;
  static const char* Domain;
  static const char* DotsHorizontal;
  static const char* DotsVertical;
  static const char* Douban;
  static const char* Download;
  static const char* Drag;
  static const char* DragHorizontal;
  static const char* DragVertical;
  static const char* Drawing;
  static const char* DrawingBox;
  static const char* Dribbble;
  static const char* DribbbleBox;
  static const char* Drone;
  static const char* Dropbox;
  static const char* Drupal;
  static const char* Duck;
  static const char* Dumbbell;
  static const char* Earth;
  static const char* EarthOff;
  static const char* Edge;
  static const char* Eject;
  static const char* ElevationDecline;
  static const char* ElevationRise;
  static const char* Elevator;
  static const char* Email;
  static const char* EmailOpen;
  static const char* EmailOpenOutline;
  static const char* EmailOutline;
  static const char* EmailSecure;
  static const char* EmailVariant;
  static const char* Emby;
  static const char* Emoticon;
  static const char* EmoticonCool;
  static const char* EmoticonDead;
  static const char* EmoticonDevil;
  static const char* EmoticonExcited;
  static const char* EmoticonHappy;
  static const char* EmoticonNeutral;
  static const char* EmoticonPoop;
  static const char* EmoticonSad;
  static const char* EmoticonTongue;
  static const char* Engine;
  static const char* EngineOutline;
  static const char* Equal;
  static const char* EqualBox;
  static const char* Eraser;
  static const char* EraserVariant;
  static const char* Escalator;
  static const char* Ethernet;
  static const char* EthernetCable;
  static const char* EthernetCableOff;
  static const char* Etsy;
  static const char* EvStation;
  static const char* Evernote;
  static const char* Exclamation;
  static const char* ExitToApp;
  static const char* Export;
  static const char* Eye;
  static const char* EyeOff;
  static const char* Eyedropper;
  static const char* EyedropperVariant;
  static const char* Face;
  static const char* FaceProfile;
  static const char* Facebook;
  static const char* FacebookBox;
  static const char* FacebookMessenger;
  static const char* Factory;
  static const char* Fan;
  static const char* FastForward;
  static const char* Fax;
  static const char* Ferry;
  static const char* File;
  static const char* FileChart;
  static const char* FileCheck;
  static const char* FileCloud;
  static const char* FileDelimited;
  static const char* FileDocument;
  static const char* FileDocumentBox;
  static const char* FileExcel;
  static const char* FileExcelBox;
  static const char* FileExport;
  static const char* FileFind;
  static const char* FileHidden;
  static const char* FileImage;
  static const char* FileImport;
  static const char* FileLock;
  static const char* FileMultiple;
  static const char* FileMusic;
  static const char* FileOutline;
  static const char* FilePdf;
  static const char* FilePdfBox;
  static const char* FilePowerpoint;
  static const char* FilePowerpointBox;
  static const char* FilePresentationBox;
  static const char* FileRestore;
  static const char* FileSend;
  static const char* FileTree;
  static const char* FileVideo;
  static const char* FileWord;
  static const char* FileWordBox;
  static const char* FileXml;
  static const char* Film;
  static const char* Filmstrip;
  static const char* FilmstripOff;
  static const char* Filter;
  static const char* FilterOutline;
  static const char* FilterRemove;
  static const char* FilterRemoveOutline;
  static const char* FilterVariant;
  static const char* Fingerprint;
  static const char* Fire;
  static const char* Firefox;
  static const char* Fish;
  static const char* Flag;
  static const char* FlagCheckered;
  static const char* FlagOutline;
  static const char* FlagOutlineVariant;
  static const char* FlagTriangle;
  static const char* FlagVariant;
  static const char* Flash;
  static const char* FlashAuto;
  static const char* FlashOff;
  static const char* FlashRedEye;
  static const char* Flashlight;
  static const char* FlashlightOff;
  static const char* Flask;
  static const char* FlaskEmpty;
  static const char* FlaskEmptyOutline;
  static const char* FlaskOutline;
  static const char* Flattr;
  static const char* FlipToBack;
  static const char* FlipToFront;
  static const char* Floppy;
  static const char* Flower;
  static const char* Folder;
  static const char* FolderAccount;
  static const char* FolderDownload;
  static const char* FolderGoogleDrive;
  static const char* FolderImage;
  static const char* FolderLock;
  static const char* FolderLockOpen;
  static const char* FolderMove;
  static const char* FolderMultiple;
  static const char* FolderMultipleImage;
  static const char* FolderMultipleOutline;
  static const char* FolderOutline;
  static const char* FolderPlus;
  static const char* FolderRemove;
  static const char* FolderStar;
  static const char* FolderUpload;
  static const char* Food;
  static const char* FoodApple;
  static const char* FoodForkDrink;
  static const char* FoodOff;
  static const char* FoodVariant;
  static const char* Football;
  static const char* FootballAustralian;
  static const char* FootballHelmet;
  static const char* FormatAlignCenter;
  static const char* FormatAlignJustify;
  static const char* FormatAlignLeft;
  static const char* FormatAlignRight;
  static const char* FormatAnnotationPlus;
  static const char* FormatBold;
  static const char* FormatClear;
  static const char* FormatColorFill;
  static const char* FormatColorText;
  static const char* FormatFloatCenter;
  static const char* FormatFloatLeft;
  static const char* FormatFloatNone;
  static const char* FormatFloatRight;
  static const char* FormatHeader1;
  static const char* FormatHeader2;
  static const char* FormatHeader3;
  static const char* FormatHeader4;
  static const char* FormatHeader5;
  static const char* FormatHeader6;
  static const char* FormatHeaderDecrease;
  static const char* FormatHeaderEqual;
  static const char* FormatHeaderIncrease;
  static const char* FormatHeaderPound;
  static const char* FormatHorizontalAlignCenter;
  static const char* FormatHorizontalAlignLeft;
  static const char* FormatHorizontalAlignRight;
  static const char* FormatIndentDecrease;
  static const char* FormatIndentIncrease;
  static const char* FormatItalic;
  static const char* FormatLineSpacing;
  static const char* FormatLineStyle;
  static const char* FormatLineWeight;
  static const char* FormatListBulleted;
  static const char* FormatListBulletedType;
  static const char* FormatListNumbers;
  static const char* FormatPaint;
  static const char* FormatParagraph;
  static const char* FormatQuote;
  static const char* FormatSection;
  static const char* FormatSize;
  static const char* FormatStrikethrough;
  static const char* FormatStrikethroughVariant;
  static const char* FormatSubscript;
  static const char* FormatSuperscript;
  static const char* FormatText;
  static const char* FormatTextdirectionLToR;
  static const char* FormatTextdirectionRToL;
  static const char* FormatTitle;
  static const char* FormatUnderline;
  static const char* FormatVerticalAlignBottom;
  static const char* FormatVerticalAlignCenter;
  static const char* FormatVerticalAlignTop;
  static const char* FormatWrapInline;
  static const char* FormatWrapSquare;
  static const char* FormatWrapTight;
  static const char* FormatWrapTopBottom;
  static const char* Forum;
  static const char* Forward;
  static const char* Foursquare;
  static const char* Fridge;
  static const char* FridgeFilled;
  static const char* FridgeFilledBottom;
  static const char* FridgeFilledTop;
  static const char* Fullscreen;
  static const char* FullscreenExit;
  static const char* Function;
  static const char* Gamepad;
  static const char* GamepadVariant;
  static const char* GasCylinder;
  static const char* GasStation;
  static const char* Gate;
  static const char* Gauge;
  static const char* Gavel;
  static const char* GenderFemale;
  static const char* GenderMale;
  static const char* GenderMaleFemale;
  static const char* GenderTransgender;
  static const char* Ghost;
  static const char* Gift;
  static const char* Git;
  static const char* GithubBox;
  static const char* GithubCircle;
  static const char* GlassFlute;
  static const char* GlassMug;
  static const char* GlassStange;
  static const char* GlassTulip;
  static const char* Glassdoor;
  static const char* Glasses;
  static const char* Gmail;
  static const char* Gnome;
  static const char* Gondola;
  static const char* Google;
  static const char* GoogleCardboard;
  static const char* GoogleChrome;
  static const char* GoogleCircles;
  static const char* GoogleCirclesCommunities;
  static const char* GoogleCirclesExtended;
  static const char* GoogleCirclesGroup;
  static const char* GoogleController;
  static const char* GoogleControllerOff;
  static const char* GoogleDrive;
  static const char* GoogleEarth;
  static const char* GoogleGlass;
  static const char* GoogleMaps;
  static const char* GoogleNearby;
  static const char* GooglePages;
  static const char* GooglePhysicalWeb;
  static const char* GooglePlay;
  static const char* GooglePlus;
  static const char* GooglePlusBox;
  static const char* GoogleTranslate;
  static const char* GoogleWallet;
  static const char* Gradient;
  static const char* GreasePencil;
  static const char* Grid;
  static const char* GridOff;
  static const char* Group;
  static const char* GuitarElectric;
  static const char* GuitarPick;
  static const char* GuitarPickOutline;
  static const char* Hackernews;
  static const char* Hamburger;
  static const char* HandPointingRight;
  static const char* Hanger;
  static const char* Hangouts;
  static const char* Harddisk;
  static const char* Headphones;
  static const char* HeadphonesBox;
  static const char* HeadphonesSettings;
  static const char* Headset;
  static const char* HeadsetDock;
  static const char* HeadsetOff;
  static const char* Heart;
  static const char* HeartBox;
  static const char* HeartBoxOutline;
  static const char* HeartBroken;
  static const char* HeartOutline;
  static const char* HeartPulse;
  static const char* Help;
  static const char* HelpCircle;
  static const char* HelpCircleOutline;
  static const char* Hexagon;
  static const char* HexagonOutline;
  static const char* Highway;
  static const char* History;
  static const char* Hololens;
  static const char* Home;
  static const char* HomeMapMarker;
  static const char* HomeModern;
  static const char* HomeOutline;
  static const char* HomeVariant;
  static const char* Hops;
  static const char* Hospital;
  static const char* HospitalBuilding;
  static const char* HospitalMarker;
  static const char* Hotel;
  static const char* Houzz;
  static const char* HouzzBox;
  static const char* Human;
  static const char* HumanChild;
  static const char* HumanFemale;
  static const char* HumanGreeting;
  static const char* HumanHandsdown;
  static const char* HumanHandsup;
  static const char* HumanMale;
  static const char* HumanMaleFemale;
  static const char* HumanPregnant;
  static const char* Image;
  static const char* ImageAlbum;
  static const char* ImageArea;
  static const char* ImageAreaClose;
  static const char* ImageBroken;
  static const char* ImageBrokenVariant;
  static const char* ImageFilter;
  static const char* ImageFilterBlackWhite;
  static const char* ImageFilterCenterFocus;
  static const char* ImageFilterCenterFocusWeak;
  static const char* ImageFilterDrama;
  static const char* ImageFilterFrames;
  static const char* ImageFilterHdr;
  static const char* ImageFilterNone;
  static const char* ImageFilterTiltShift;
  static const char* ImageFilterVintage;
  static const char* ImageMultiple;
  static const char* Import;
  static const char* Inbox;
  static const char* InboxArrowDown;
  static const char* InboxArrowUp;
  static const char* Incognito;
  static const char* Information;
  static const char* InformationOutline;
  static const char* InformationVariant;
  static const char* Instagram;
  static const char* Instapaper;
  static const char* InternetExplorer;
  static const char* InvertColors;
  static const char* Itunes;
  static const char* Jeepney;
  static const char* Jira;
  static const char* Jsfiddle;
  static const char* Json;
  static const char* Keg;
  static const char* Kettle;
  static const char* Key;
  static const char* KeyChange;
  static const char* KeyMinus;
  static const char* KeyPlus;
  static const char* KeyRemove;
  static const char* KeyVariant;
  static const char* Keyboard;
  static const char* KeyboardBackspace;
  static const char* KeyboardCaps;
  static const char* KeyboardClose;
  static const char* KeyboardOff;
  static const char* KeyboardReturn;
  static const char* KeyboardTab;
  static const char* KeyboardVariant;
  static const char* Kodi;
  static const char* Label;
  static const char* LabelOutline;
  static const char* Lambda;
  static const char* Lamp;
  static const char* Lan;
  static const char* LanConnect;
  static const char* LanDisconnect;
  static const char* LanPending;
  static const char* LanguageC;
  static const char* LanguageCpp;
  static const char* LanguageCsharp;
  static const char* LanguageCss3;
  static const char* LanguageHtml5;
  static const char* LanguageJavascript;
  static const char* LanguagePhp;
  static const char* LanguagePython;
  static const char* LanguagePythonText;
  static const char* Laptop;
  static const char* LaptopChromebook;
  static const char* LaptopMac;
  static const char* LaptopWindows;
  static const char* Lastfm;
  static const char* Launch;
  static const char* Layers;
  static const char* LayersOff;
  static const char* LeadPencil;
  static const char* Leaf;
  static const char* LedOff;
  static const char* LedOn;
  static const char* LedOutline;
  static const char* LedVariantOff;
  static const char* LedVariantOn;
  static const char* LedVariantOutline;
  static const char* Library;
  static const char* LibraryBooks;
  static const char* LibraryMusic;
  static const char* LibraryPlus;
  static const char* Lightbulb;
  static const char* LightbulbOutline;
  static const char* Link;
  static const char* LinkOff;
  static const char* LinkVariant;
  static const char* LinkVariantOff;
  static const char* Linkedin;
  static const char* LinkedinBox;
  static const char* Linux;
  static const char* Lock;
  static const char* LockOpen;
  static const char* LockOpenOutline;
  static const char* LockOutline;
  static const char* LockPlus;
  static const char* Login;
  static const char* LoginVariant;
  static const char* Logout;
  static const char* LogoutVariant;
  static const char* Looks;
  static const char* Loupe;
  static const char* Lumx;
  static const char* Magnet;
  static const char* MagnetOn;
  static const char* Magnify;
  static const char* MagnifyMinus;
  static const char* MagnifyPlus;
  static const char* MailRu;
  static const char* Map;
  static const char* MapMarker;
  static const char* MapMarkerCircle;
  static const char* MapMarkerMinus;
  static const char* MapMarkerMultiple;
  static const char* MapMarkerOff;
  static const char* MapMarkerPlus;
  static const char* MapMarkerRadius;
  static const char* Margin;
  static const char* Markdown;
  static const char* Marker;
  static const char* MarkerCheck;
  static const char* Martini;
  static const char* MaterialUi;
  static const char* MathCompass;
  static const char* Matrix;
  static const char* Maxcdn;
  static const char* Medium;
  static const char* Memory;
  static const char* Menu;
  static const char* MenuDown;
  static const char* MenuDownOutline;
  static const char* MenuLeft;
  static const char* MenuRight;
  static const char* MenuUp;
  static const char* MenuUpOutline;
  static const char* Message;
  static const char* MessageAlert;
  static const char* MessageBulleted;
  static const char* MessageBulletedOff;
  static const char* MessageDraw;
  static const char* MessageImage;
  static const char* MessageOutline;
  static const char* MessagePlus;
  static const char* MessageProcessing;
  static const char* MessageReply;
  static const char* MessageReplyText;
  static const char* MessageText;
  static const char* MessageTextOutline;
  static const char* MessageVideo;
  static const char* Meteor;
  static const char* Microphone;
  static const char* MicrophoneOff;
  static const char* MicrophoneOutline;
  static const char* MicrophoneSettings;
  static const char* MicrophoneVariant;
  static const char* MicrophoneVariantOff;
  static const char* Microscope;
  static const char* Microsoft;
  static const char* Minecraft;
  static const char* Minus;
  static const char* MinusBox;
  static const char* MinusCircle;
  static const char* MinusCircleOutline;
  static const char* MinusNetwork;
  static const char* Mixcloud;
  static const char* Monitor;
  static const char* MonitorMultiple;
  static const char* More;
  static const char* Motorbike;
  static const char* Mouse;
  static const char* MouseOff;
  static const char* MouseVariant;
  static const char* MouseVariantOff;
  static const char* MoveResize;
  static const char* MoveResizeVariant;
  static const char* Movie;
  static const char* Multiplication;
  static const char* MultiplicationBox;
  static const char* MusicBox;
  static const char* MusicBoxOutline;
  static const char* MusicCircle;
  static const char* MusicNote;
  static const char* MusicNoteBluetooth;
  static const char* MusicNoteBluetoothOff;
  static const char* MusicNoteEighth;
  static const char* MusicNoteHalf;
  static const char* MusicNoteOff;
  static const char* MusicNoteQuarter;
  static const char* MusicNoteSixteenth;
  static const char* MusicNoteWhole;
  static const char* Nature;
  static const char* NaturePeople;
  static const char* Navigation;
  static const char* NearMe;
  static const char* Needle;
  static const char* NestProtect;
  static const char* NestThermostat;
  static const char* NewBox;
  static const char* Newspaper;
  static const char* Nfc;
  static const char* NfcTap;
  static const char* NfcVariant;
  static const char* Nodejs;
  static const char* Note;
  static const char* NoteMultiple;
  static const char* NoteMultipleOutline;
  static const char* NoteOutline;
  static const char* NotePlus;
  static const char* NotePlusOutline;
  static const char* NoteText;
  static const char* NotificationClearAll;
  static const char* Nuke;
  static const char* Numeric;
  static const char* Numeric0Box;
  static const char* Numeric0BoxMultipleOutline;
  static const char* Numeric0BoxOutline;
  static const char* Numeric1Box;
  static const char* Numeric1BoxMultipleOutline;
  static const char* Numeric1BoxOutline;
  static const char* Numeric2Box;
  static const char* Numeric2BoxMultipleOutline;
  static const char* Numeric2BoxOutline;
  static const char* Numeric3Box;
  static const char* Numeric3BoxMultipleOutline;
  static const char* Numeric3BoxOutline;
  static const char* Numeric4Box;
  static const char* Numeric4BoxMultipleOutline;
  static const char* Numeric4BoxOutline;
  static const char* Numeric5Box;
  static const char* Numeric5BoxMultipleOutline;
  static const char* Numeric5BoxOutline;
  static const char* Numeric6Box;
  static const char* Numeric6BoxMultipleOutline;
  static const char* Numeric6BoxOutline;
  static const char* Numeric7Box;
  static const char* Numeric7BoxMultipleOutline;
  static const char* Numeric7BoxOutline;
  static const char* Numeric8Box;
  static const char* Numeric8BoxMultipleOutline;
  static const char* Numeric8BoxOutline;
  static const char* Numeric9Box;
  static const char* Numeric9BoxMultipleOutline;
  static const char* Numeric9BoxOutline;
  static const char* Numeric9PlusBox;
  static const char* Numeric9PlusBoxMultipleOutline;
  static const char* Numeric9PlusBoxOutline;
  static const char* Nutrition;
  static const char* Oar;
  static const char* Octagon;
  static const char* OctagonOutline;
  static const char* Odnoklassniki;
  static const char* Office;
  static const char* Oil;
  static const char* OilTemperature;
  static const char* Omega;
  static const char* Onedrive;
  static const char* Opacity;
  static const char* OpenInApp;
  static const char* OpenInNew;
  static const char* Openid;
  static const char* Opera;
  static const char* Ornament;
  static const char* OrnamentVariant;
  static const char* Owl;
  static const char* Package;
  static const char* PackageDown;
  static const char* PackageUp;
  static const char* PackageVariant;
  static const char* PackageVariantClosed;
  static const char* PageFirst;
  static const char* PageLast;
  static const char* Palette;
  static const char* PaletteAdvanced;
  static const char* Panda;
  static const char* Pandora;
  static const char* Panorama;
  static const char* PanoramaFisheye;
  static const char* PanoramaHorizontal;
  static const char* PanoramaVertical;
  static const char* PanoramaWideAngle;
  static const char* PaperCutVertical;
  static const char* Paperclip;
  static const char* Parking;
  static const char* Pause;
  static const char* PauseCircle;
  static const char* PauseCircleOutline;
  static const char* PauseOctagon;
  static const char* PauseOctagonOutline;
  static const char* Paw;
  static const char* PawOff;
  static const char* Pen;
  static const char* Pencil;
  static const char* PencilBox;
  static const char* PencilBoxOutline;
  static const char* PencilLock;
  static const char* PencilOff;
  static const char* Percent;
  static const char* Pharmacy;
  static const char* Phone;
  static const char* PhoneBluetooth;
  static const char* PhoneClassic;
  static const char* PhoneForward;
  static const char* PhoneHangup;
  static const char* PhoneInTalk;
  static const char* PhoneIncoming;
  static const char* PhoneLocked;
  static const char* PhoneLog;
  static const char* PhoneMinus;
  static const char* PhoneMissed;
  static const char* PhoneOutgoing;
  static const char* PhonePaused;
  static const char* PhonePlus;
  static const char* PhoneSettings;
  static const char* PhoneVoip;
  static const char* Pi;
  static const char* PiBox;
  static const char* Piano;
  static const char* Pig;
  static const char* Pill;
  static const char* Pin;
  static const char* PinOff;
  static const char* PineTree;
  static const char* PineTreeBox;
  static const char* Pinterest;
  static const char* PinterestBox;
  static const char* Pizza;
  static const char* PlaneShield;
  static const char* Play;
  static const char* PlayBoxOutline;
  static const char* PlayCircle;
  static const char* PlayCircleOutline;
  static const char* PlayPause;
  static const char* PlayProtectedContent;
  static const char* PlaylistCheck;
  static const char* PlaylistMinus;
  static const char* PlaylistPlay;
  static const char* PlaylistPlus;
  static const char* PlaylistRemove;
  static const char* Playstation;
  static const char* Plex;
  static const char* Plus;
  static const char* PlusBox;
  static const char* PlusCircle;
  static const char* PlusCircleMultipleOutline;
  static const char* PlusCircleOutline;
  static const char* PlusNetwork;
  static const char* PlusOne;
  static const char* Pocket;
  static const char* Pokeball;
  static const char* Polaroid;
  static const char* Poll;
  static const char* PollBox;
  static const char* Polymer;
  static const char* Pool;
  static const char* Popcorn;
  static const char* Pot;
  static const char* PotMix;
  static const char* Pound;
  static const char* PoundBox;
  static const char* Power;
  static const char* PowerPlug;
  static const char* PowerPlugOff;
  static const char* PowerSettings;
  static const char* PowerSocket;
  static const char* Presentation;
  static const char* PresentationPlay;
  static const char* Printer;
  static const char* Printer3d;
  static const char* PrinterAlert;
  static const char* PriorityHigh;
  static const char* PriorityLow;
  static const char* ProfessionalHexagon;
  static const char* Projector;
  static const char* ProjectorScreen;
  static const char* Publish;
  static const char* Pulse;
  static const char* Puzzle;
  static const char* Qqchat;
  static const char* Qrcode;
  static const char* QrcodeScan;
  static const char* Quadcopter;
  static const char* QualityHigh;
  static const char* Quicktime;
  static const char* Radar;
  static const char* Radiator;
  static const char* Radio;
  static const char* RadioHandheld;
  static const char* RadioTower;
  static const char* Radioactive;
  static const char* RadioboxBlank;
  static const char* RadioboxMarked;
  static const char* Raspberrypi;
  static const char* RayEnd;
  static const char* RayEndArrow;
  static const char* RayStart;
  static const char* RayStartArrow;
  static const char* RayStartEnd;
  static const char* RayVertex;
  static const char* Rdio;
  static const char* Read;
  static const char* Readability;
  static const char* Receipt;
  static const char* Record;
  static const char* RecordRec;
  static const char* Recycle;
  static const char* Reddit;
  static const char* Redo;
  static const char* RedoVariant;
  static const char* Refresh;
  static const char* Regex;
  static const char* RelativeScale;
  static const char* Reload;
  static const char* Remote;
  static const char* RenameBox;
  static const char* ReorderHorizontal;
  static const char* ReorderVertical;
  static const char* Repeat;
  static const char* RepeatOff;
  static const char* RepeatOnce;
  static const char* Replay;
  static const char* Reply;
  static const char* ReplyAll;
  static const char* Reproduction;
  static const char* ResizeBottomRight;
  static const char* Responsive;
  static const char* Restore;
  static const char* Rewind;
  static const char* Ribbon;
  static const char* Road;
  static const char* RoadVariant;
  static const char* Robot;
  static const char* Rocket;
  static const char* Rotate3d;
  static const char* Rotate90;
  static const char* RotateLeft;
  static const char* RotateLeftVariant;
  static const char* RotateRight;
  static const char* RotateRightVariant;
  static const char* RoundedCorner;
  static const char* RouterWireless;
  static const char* Routes;
  static const char* Rowing;
  static const char* Rss;
  static const char* RssBox;
  static const char* Ruler;
  static const char* Run;
  static const char* Sale;
  static const char* Satellite;
  static const char* SatelliteVariant;
  static const char* Saxophone;
  static const char* Scale;
  static const char* ScaleBalance;
  static const char* ScaleBathroom;
  static const char* Scanner;
  static const char* School;
  static const char* ScreenRotation;
  static const char* ScreenRotationLock;
  static const char* Screwdriver;
  static const char* Script;
  static const char* Sd;
  static const char* Seal;
  static const char* SeatFlat;
  static const char* SeatFlatAngled;
  static const char* SeatIndividualSuite;
  static const char* SeatLegroomExtra;
  static const char* SeatLegroomNormal;
  static const char* SeatLegroomReduced;
  static const char* SeatReclineExtra;
  static const char* SeatReclineNormal;
  static const char* Security;
  static const char* SecurityHome;
  static const char* SecurityNetwork;
  static const char* Select;
  static const char* SelectAll;
  static const char* SelectInverse;
  static const char* SelectOff;
  static const char* Selection;
  static const char* Send;
  static const char* SerialPort;
  static const char* Server;
  static const char* ServerMinus;
  static const char* ServerNetwork;
  static const char* ServerNetworkOff;
  static const char* ServerOff;
  static const char* ServerPlus;
  static const char* ServerRemove;
  static const char* ServerSecurity;
  static const char* Settings;
  static const char* SettingsBox;
  static const char* ShapeCirclePlus;
  static const char* ShapePlus;
  static const char* ShapePolygonPlus;
  static const char* ShapeRectanglePlus;
  static const char* ShapeSquarePlus;
  static const char* Share;
  static const char* ShareVariant;
  static const char* Shield;
  static const char* ShieldOutline;
  static const char* Shopping;
  static const char* ShoppingMusic;
  static const char* Shredder;
  static const char* Shuffle;
  static const char* ShuffleDisabled;
  static const char* ShuffleVariant;
  static const char* Sigma;
  static const char* SigmaLower;
  static const char* SignCaution;
  static const char* Signal;
  static const char* SignalVariant;
  static const char* Silverware;
  static const char* SilverwareFork;
  static const char* SilverwareSpoon;
  static const char* SilverwareVariant;
  static const char* Sim;
  static const char* SimAlert;
  static const char* SimOff;
  static const char* Sitemap;
  static const char* SkipBackward;
  static const char* SkipForward;
  static const char* SkipNext;
  static const char* SkipNextCircle;
  static const char* SkipNextCircleOutline;
  static const char* SkipPrevious;
  static const char* SkipPreviousCircle;
  static const char* SkipPreviousCircleOutline;
  static const char* Skull;
  static const char* Skype;
  static const char* SkypeBusiness;
  static const char* Slack;
  static const char* Sleep;
  static const char* SleepOff;
  static const char* Smoking;
  static const char* SmokingOff;
  static const char* Snapchat;
  static const char* Snowman;
  static const char* Soccer;
  static const char* Sofa;
  static const char* Solid;
  static const char* Sort;
  static const char* SortAlphabetical;
  static const char* SortAscending;
  static const char* SortDescending;
  static const char* SortNumeric;
  static const char* SortVariant;
  static const char* Soundcloud;
  static const char* SourceBranch;
  static const char* SourceFork;
  static const char* SourceMerge;
  static const char* SourcePull;
  static const char* Speaker;
  static const char* SpeakerOff;
  static const char* Speedometer;
  static const char* Spellcheck;
  static const char* Spotify;
  static const char* Spotlight;
  static const char* SpotlightBeam;
  static const char* Spray;
  static const char* SquareInc;
  static const char* SquareIncCash;
  static const char* Stackexchange;
  static const char* Stackoverflow;
  static const char* Stairs;
  static const char* Star;
  static const char* StarCircle;
  static const char* StarHalf;
  static const char* StarOff;
  static const char* StarOutline;
  static const char* Steam;
  static const char* Steering;
  static const char* StepBackward;
  static const char* StepBackward2;
  static const char* StepForward;
  static const char* StepForward2;
  static const char* Stethoscope;
  static const char* Sticker;
  static const char* Stocking;
  static const char* Stop;
  static const char* StopCircle;
  static const char* StopCircleOutline;
  static const char* Store;
  static const char* Store24Hour;
  static const char* Stove;
  static const char* SubdirectoryArrowLeft;
  static const char* SubdirectoryArrowRight;
  static const char* Subway;
  static const char* SubwayVariant;
  static const char* Sunglasses;
  static const char* SurroundSound;
  static const char* SwapHorizontal;
  static const char* SwapVertical;
  static const char* Swim;
  static const char* Switch;
  static const char* Sword;
  static const char* Sync;
  static const char* SyncAlert;
  static const char* SyncOff;
  static const char* Tab;
  static const char* TabUnselected;
  static const char* Table;
  static const char* TableColumnPlusAfter;
  static const char* TableColumnPlusBefore;
  static const char* TableColumnRemove;
  static const char* TableColumnWidth;
  static const char* TableEdit;
  static const char* TableLarge;
  static const char* TableRowHeight;
  static const char* TableRowPlusAfter;
  static const char* TableRowPlusBefore;
  static const char* TableRowRemove;
  static const char* Tablet;
  static const char* TabletAndroid;
  static const char* TabletIpad;
  static const char* Tag;
  static const char* TagFaces;
  static const char* TagHeart;
  static const char* TagMultiple;
  static const char* TagOutline;
  static const char* TagTextOutline;
  static const char* Target;
  static const char* Taxi;
  static const char* Teamviewer;
  static const char* Telegram;
  static const char* Television;
  static const char* TelevisionGuide;
  static const char* TemperatureCelsius;
  static const char* TemperatureFahrenheit;
  static const char* TemperatureKelvin;
  static const char* Tennis;
  static const char* Tent;
  static const char* Terrain;
  static const char* TestTube;
  static const char* TextShadow;
  static const char* TextToSpeech;
  static const char* TextToSpeechOff;
  static const char* Textbox;
  static const char* Texture;
  static const char* Theater;
  static const char* ThemeLightDark;
  static const char* Thermometer;
  static const char* ThermometerLines;
  static const char* ThumbDown;
  static const char* ThumbDownOutline;
  static const char* ThumbUp;
  static const char* ThumbUpOutline;
  static const char* ThumbsUpDown;
  static const char* Ticket;
  static const char* TicketAccount;
  static const char* TicketConfirmation;
  static const char* Tie;
  static const char* Timelapse;
  static const char* Timer;
  static const char* Timer10;
  static const char* Timer3;
  static const char* TimerOff;
  static const char* TimerSand;
  static const char* TimerSandEmpty;
  static const char* Timetable;
  static const char* ToggleSwitch;
  static const char* ToggleSwitchOff;
  static const char* Tooltip;
  static const char* TooltipEdit;
  static const char* TooltipImage;
  static const char* TooltipOutline;
  static const char* TooltipOutlinePlus;
  static const char* TooltipText;
  static const char* Tooth;
  static const char* Tor;
  static const char* TowerBeach;
  static const char* TowerFire;
  static const char* TrafficLight;
  static const char* Train;
  static const char* Tram;
  static const char* Transcribe;
  static const char* TranscribeClose;
  static const char* Transfer;
  static const char* TransitTransfer;
  static const char* Translate;
  static const char* Tree;
  static const char* Trello;
  static const char* TrendingDown;
  static const char* TrendingNeutral;
  static const char* TrendingUp;
  static const char* Triangle;
  static const char* TriangleOutline;
  static const char* Trophy;
  static const char* TrophyAward;
  static const char* TrophyOutline;
  static const char* TrophyVariant;
  static const char* TrophyVariantOutline;
  static const char* Truck;
  static const char* TruckDelivery;
  static const char* TshirtCrew;
  static const char* TshirtV;
  static const char* Tumblr;
  static const char* TumblrReblog;
  static const char* Tune;
  static const char* TuneVertical;
  static const char* Twitch;
  static const char* Twitter;
  static const char* TwitterBox;
  static const char* TwitterCircle;
  static const char* TwitterRetweet;
  static const char* Ubuntu;
  static const char* Umbraco;
  static const char* Umbrella;
  static const char* UmbrellaOutline;
  static const char* Undo;
  static const char* UndoVariant;
  static const char* UnfoldLess;
  static const char* UnfoldMore;
  static const char* Ungroup;
  static const char* Unity;
  static const char* Untappd;
  static const char* Update;
  static const char* Upload;
  static const char* Usb;
  static const char* VectorArrangeAbove;
  static const char* VectorArrangeBelow;
  static const char* VectorCircle;
  static const char* VectorCircleVariant;
  static const char* VectorCombine;
  static const char* VectorCurve;
  static const char* VectorDifference;
  static const char* VectorDifferenceAb;
  static const char* VectorDifferenceBa;
  static const char* VectorIntersection;
  static const char* VectorLine;
  static const char* VectorPoint;
  static const char* VectorPolygon;
  static const char* VectorPolyline;
  static const char* VectorRectangle;
  static const char* VectorSelection;
  static const char* VectorSquare;
  static const char* VectorTriangle;
  static const char* VectorUnion;
  static const char* Verified;
  static const char* Vibrate;
  static const char* Video;
  static const char* VideoOff;
  static const char* VideoSwitch;
  static const char* ViewAgenda;
  static const char* ViewArray;
  static const char* ViewCarousel;
  static const char* ViewColumn;
  static const char* ViewDashboard;
  static const char* ViewDay;
  static const char* ViewGrid;
  static const char* ViewHeadline;
  static const char* ViewList;
  static const char* ViewModule;
  static const char* ViewQuilt;
  static const char* ViewStream;
  static const char* ViewWeek;
  static const char* Vimeo;
  static const char* Vine;
  static const char* Violin;
  static const char* Visualstudio;
  static const char* Vk;
  static const char* VkBox;
  static const char* VkCircle;
  static const char* Vlc;
  static const char* Voice;
  static const char* Voicemail;
  static const char* VolumeHigh;
  static const char* VolumeLow;
  static const char* VolumeMedium;
  static const char* VolumeOff;
  static const char* Vpn;
  static const char* Walk;
  static const char* Wallet;
  static const char* WalletGiftcard;
  static const char* WalletMembership;
  static const char* WalletTravel;
  static const char* Wan;
  static const char* Watch;
  static const char* WatchExport;
  static const char* WatchImport;
  static const char* WatchVibrate;
  static const char* Water;
  static const char* WaterOff;
  static const char* WaterPercent;
  static const char* WaterPump;
  static const char* Watermark;
  static const char* WeatherCloudy;
  static const char* WeatherFog;
  static const char* WeatherHail;
  static const char* WeatherLightning;
  static const char* WeatherLightningRainy;
  static const char* WeatherNight;
  static const char* WeatherPartlycloudy;
  static const char* WeatherPouring;
  static const char* WeatherRainy;
  static const char* WeatherSnowy;
  static const char* WeatherSnowyRainy;
  static const char* WeatherSunny;
  static const char* WeatherSunset;
  static const char* WeatherSunsetDown;
  static const char* WeatherSunsetUp;
  static const char* WeatherWindy;
  static const char* WeatherWindyVariant;
  static const char* Web;
  static const char* Webcam;
  static const char* Webhook;
  static const char* Wechat;
  static const char* Weight;
  static const char* WeightKilogram;
  static const char* Whatsapp;
  static const char* WheelchairAccessibility;
  static const char* WhiteBalanceAuto;
  static const char* WhiteBalanceIncandescent;
  static const char* WhiteBalanceIridescent;
  static const char* WhiteBalanceSunny;
  static const char* Wifi;
  static const char* WifiOff;
  static const char* Wii;
  static const char* Wikipedia;
  static const char* WindowClose;
  static const char* WindowClosed;
  static const char* WindowMaximize;
  static const char* WindowMinimize;
  static const char* WindowOpen;
  static const char* WindowRestore;
  static const char* Windows;
  static const char* Wordpress;
  static const char* Worker;
  static const char* Wrap;
  static const char* Wrench;
  static const char* Wunderlist;
  static const char* Xaml;
  static const char* Xbox;
  static const char* XboxController;
  static const char* XboxControllerOff;
  static const char* Xda;
  static const char* Xing;
  static const char* XingBox;
  static const char* XingCircle;
  static const char* Xml;
  static const char* Yeast;
  static const char* Yelp;
  static const char* YinYang;
  static const char* YoutubePlay;
  static const char* ZipBox;

};

};
