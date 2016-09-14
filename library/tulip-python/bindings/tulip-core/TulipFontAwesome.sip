/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

class TulipFontAwesome {

%TypeHeaderCode
#include <tulip/TulipFontAwesome.h>
%End

%Docstring

.. versionadded:: 4.8

Helper class for the configuration of a Font Awesome glyph.

`Font Awesome <http://fontawesome.io>`_ is a free iconic font by Dave Gandy
offering more than 500 customizable scalable vector icons.

A glyph has been added to Tulip enabling to use these great icons
as nodes and edges extremities shapes.

That class offers utility functions and supported icons names constants.

To set a node glyph as a Font Awesome icon, the Font Awesome glyph must be associated
to the node through the modification of the "viewShape" integer property attached to the graph.
The name of the icon to use must then be set in the "viewFontAwesomeIcon" string property.
As an example, the following code snippet activates the Font Awesome glyph for all nodes
and sets the "user" icon (:faicon:`user`)::

  # graph is a pointer to a tlp.Graph object
  viewShape = graph.getIntegerProperty("viewShape")
  viewFontAwesomeIcon = graph.getStringProperty("viewFontAwesomeIcon")

  # sets the Font Awesome glyph on all nodes
  viewShape.setAllNodeValue(tlp.NodeShape.FontAwesomeIcon)
  # sets the "user" glyph for all nodes
  viewFontAwesomeIcon.setAllNodeValue(tlp.TulipFontAwesome.User)

The following string constants representing the supported icons are available from Tulip 4.8.0 (based on Font Awesome 4.4):

  * :const:`tlp.TulipFontAwesome.Px500` (:faicon:`500px`)
  * :const:`tlp.TulipFontAwesome.Adjust` (:faicon:`adjust`)
  * :const:`tlp.TulipFontAwesome.Adn` (:faicon:`adn`)
  * :const:`tlp.TulipFontAwesome.AlignCenter` (:faicon:`align-center`)
  * :const:`tlp.TulipFontAwesome.AlignJustify` (:faicon:`align-justify`)
  * :const:`tlp.TulipFontAwesome.AlignLeft` (:faicon:`align-left`)
  * :const:`tlp.TulipFontAwesome.AlignRight` (:faicon:`align-right`)
  * :const:`tlp.TulipFontAwesome.Amazon` (:faicon:`amazon`)
  * :const:`tlp.TulipFontAwesome.Ambulance` (:faicon:`ambulance`)
  * :const:`tlp.TulipFontAwesome.Anchor` (:faicon:`anchor`)
  * :const:`tlp.TulipFontAwesome.Android` (:faicon:`android`)
  * :const:`tlp.TulipFontAwesome.Angellist` (:faicon:`angellist`)
  * :const:`tlp.TulipFontAwesome.AngleDoubleDown` (:faicon:`angle-double-down`)
  * :const:`tlp.TulipFontAwesome.AngleDoubleLeft` (:faicon:`angle-double-left`)
  * :const:`tlp.TulipFontAwesome.AngleDoubleRight` (:faicon:`angle-double-right`)
  * :const:`tlp.TulipFontAwesome.AngleDoubleUp` (:faicon:`angle-double-up`)
  * :const:`tlp.TulipFontAwesome.AngleDown` (:faicon:`angle-down`)
  * :const:`tlp.TulipFontAwesome.AngleLeft` (:faicon:`angle-left`)
  * :const:`tlp.TulipFontAwesome.AngleRight` (:faicon:`angle-right`)
  * :const:`tlp.TulipFontAwesome.AngleUp` (:faicon:`angle-up`)
  * :const:`tlp.TulipFontAwesome.Apple` (:faicon:`apple`)
  * :const:`tlp.TulipFontAwesome.Archive` (:faicon:`archive`)
  * :const:`tlp.TulipFontAwesome.AreaChart` (:faicon:`area-chart`)
  * :const:`tlp.TulipFontAwesome.ArrowCircleDown` (:faicon:`arrow-circle-down`)
  * :const:`tlp.TulipFontAwesome.ArrowCircleLeft` (:faicon:`arrow-circle-left`)
  * :const:`tlp.TulipFontAwesome.ArrowCircleODown` (:faicon:`arrow-circle-o-down`)
  * :const:`tlp.TulipFontAwesome.ArrowCircleOLeft` (:faicon:`arrow-circle-o-left`)
  * :const:`tlp.TulipFontAwesome.ArrowCircleORight` (:faicon:`arrow-circle-o-right`)
  * :const:`tlp.TulipFontAwesome.ArrowCircleOUp` (:faicon:`arrow-circle-o-up`)
  * :const:`tlp.TulipFontAwesome.ArrowCircleRight` (:faicon:`arrow-circle-right`)
  * :const:`tlp.TulipFontAwesome.ArrowCircleUp` (:faicon:`arrow-circle-up`)
  * :const:`tlp.TulipFontAwesome.ArrowDown` (:faicon:`arrow-down`)
  * :const:`tlp.TulipFontAwesome.ArrowLeft` (:faicon:`arrow-left`)
  * :const:`tlp.TulipFontAwesome.ArrowRight` (:faicon:`arrow-right`)
  * :const:`tlp.TulipFontAwesome.ArrowUp` (:faicon:`arrow-up`)
  * :const:`tlp.TulipFontAwesome.Arrows` (:faicon:`arrows`)
  * :const:`tlp.TulipFontAwesome.ArrowsAlt` (:faicon:`arrows-alt`)
  * :const:`tlp.TulipFontAwesome.ArrowsH` (:faicon:`arrows-h`)
  * :const:`tlp.TulipFontAwesome.ArrowsV` (:faicon:`arrows-v`)
  * :const:`tlp.TulipFontAwesome.Asterisk` (:faicon:`asterisk`)
  * :const:`tlp.TulipFontAwesome.At` (:faicon:`at`)
  * :const:`tlp.TulipFontAwesome.Automobile` (:faicon:`automobile`)
  * :const:`tlp.TulipFontAwesome.Backward` (:faicon:`backward`)
  * :const:`tlp.TulipFontAwesome.BalanceScale` (:faicon:`balance-scale`)
  * :const:`tlp.TulipFontAwesome.Ban` (:faicon:`ban`)
  * :const:`tlp.TulipFontAwesome.Bank` (:faicon:`bank`)
  * :const:`tlp.TulipFontAwesome.BarChart` (:faicon:`bar-chart`)
  * :const:`tlp.TulipFontAwesome.BarChartO` (:faicon:`bar-chart-o`)
  * :const:`tlp.TulipFontAwesome.Barcode` (:faicon:`barcode`)
  * :const:`tlp.TulipFontAwesome.Bars` (:faicon:`bars`)
  * :const:`tlp.TulipFontAwesome.Battery0` (:faicon:`battery-0`)
  * :const:`tlp.TulipFontAwesome.Battery1` (:faicon:`battery-1`)
  * :const:`tlp.TulipFontAwesome.Battery2` (:faicon:`battery-2`)
  * :const:`tlp.TulipFontAwesome.Battery3` (:faicon:`battery-3`)
  * :const:`tlp.TulipFontAwesome.Battery4` (:faicon:`battery-4`)
  * :const:`tlp.TulipFontAwesome.BatteryEmpty` (:faicon:`battery-empty`)
  * :const:`tlp.TulipFontAwesome.BatteryFull` (:faicon:`battery-full`)
  * :const:`tlp.TulipFontAwesome.BatteryHalf` (:faicon:`battery-half`)
  * :const:`tlp.TulipFontAwesome.BatteryQuarter` (:faicon:`battery-quarter`)
  * :const:`tlp.TulipFontAwesome.BatteryThreeQuarters` (:faicon:`battery-three-quarters`)
  * :const:`tlp.TulipFontAwesome.Bed` (:faicon:`bed`)
  * :const:`tlp.TulipFontAwesome.Beer` (:faicon:`beer`)
  * :const:`tlp.TulipFontAwesome.Behance` (:faicon:`behance`)
  * :const:`tlp.TulipFontAwesome.BehanceSquare` (:faicon:`behance-square`)
  * :const:`tlp.TulipFontAwesome.Bell` (:faicon:`bell`)
  * :const:`tlp.TulipFontAwesome.BellO` (:faicon:`bell-o`)
  * :const:`tlp.TulipFontAwesome.BellSlash` (:faicon:`bell-slash`)
  * :const:`tlp.TulipFontAwesome.BellSlashO` (:faicon:`bell-slash-o`)
  * :const:`tlp.TulipFontAwesome.Bicycle` (:faicon:`bicycle`)
  * :const:`tlp.TulipFontAwesome.Binoculars` (:faicon:`binoculars`)
  * :const:`tlp.TulipFontAwesome.BirthdayCake` (:faicon:`birthday-cake`)
  * :const:`tlp.TulipFontAwesome.Bitbucket` (:faicon:`bitbucket`)
  * :const:`tlp.TulipFontAwesome.BitbucketSquare` (:faicon:`bitbucket-square`)
  * :const:`tlp.TulipFontAwesome.Bitcoin` (:faicon:`bitcoin`)
  * :const:`tlp.TulipFontAwesome.BlackTie` (:faicon:`black-tie`)
  * :const:`tlp.TulipFontAwesome.Bold` (:faicon:`bold`)
  * :const:`tlp.TulipFontAwesome.Bolt` (:faicon:`bolt`)
  * :const:`tlp.TulipFontAwesome.Bomb` (:faicon:`bomb`)
  * :const:`tlp.TulipFontAwesome.Book` (:faicon:`book`)
  * :const:`tlp.TulipFontAwesome.Bookmark` (:faicon:`bookmark`)
  * :const:`tlp.TulipFontAwesome.BookmarkO` (:faicon:`bookmark-o`)
  * :const:`tlp.TulipFontAwesome.Briefcase` (:faicon:`briefcase`)
  * :const:`tlp.TulipFontAwesome.Btc` (:faicon:`btc`)
  * :const:`tlp.TulipFontAwesome.Bug` (:faicon:`bug`)
  * :const:`tlp.TulipFontAwesome.Building` (:faicon:`building`)
  * :const:`tlp.TulipFontAwesome.BuildingO` (:faicon:`building-o`)
  * :const:`tlp.TulipFontAwesome.Bullhorn` (:faicon:`bullhorn`)
  * :const:`tlp.TulipFontAwesome.Bullseye` (:faicon:`bullseye`)
  * :const:`tlp.TulipFontAwesome.Bus` (:faicon:`bus`)
  * :const:`tlp.TulipFontAwesome.Buysellads` (:faicon:`buysellads`)
  * :const:`tlp.TulipFontAwesome.Cab` (:faicon:`cab`)
  * :const:`tlp.TulipFontAwesome.Calculator` (:faicon:`calculator`)
  * :const:`tlp.TulipFontAwesome.Calendar` (:faicon:`calendar`)
  * :const:`tlp.TulipFontAwesome.CalendarCheckO` (:faicon:`calendar-check-o`)
  * :const:`tlp.TulipFontAwesome.CalendarMinusO` (:faicon:`calendar-minus-o`)
  * :const:`tlp.TulipFontAwesome.CalendarO` (:faicon:`calendar-o`)
  * :const:`tlp.TulipFontAwesome.CalendarPlusO` (:faicon:`calendar-plus-o`)
  * :const:`tlp.TulipFontAwesome.CalendarTimesO` (:faicon:`calendar-times-o`)
  * :const:`tlp.TulipFontAwesome.Camera` (:faicon:`camera`)
  * :const:`tlp.TulipFontAwesome.CameraRetro` (:faicon:`camera-retro`)
  * :const:`tlp.TulipFontAwesome.Car` (:faicon:`car`)
  * :const:`tlp.TulipFontAwesome.CaretDown` (:faicon:`caret-down`)
  * :const:`tlp.TulipFontAwesome.CaretLeft` (:faicon:`caret-left`)
  * :const:`tlp.TulipFontAwesome.CaretRight` (:faicon:`caret-right`)
  * :const:`tlp.TulipFontAwesome.CaretSquareODown` (:faicon:`caret-square-o-down`)
  * :const:`tlp.TulipFontAwesome.CaretSquareOLeft` (:faicon:`caret-square-o-left`)
  * :const:`tlp.TulipFontAwesome.CaretSquareORight` (:faicon:`caret-square-o-right`)
  * :const:`tlp.TulipFontAwesome.CaretSquareOUp` (:faicon:`caret-square-o-up`)
  * :const:`tlp.TulipFontAwesome.CaretUp` (:faicon:`caret-up`)
  * :const:`tlp.TulipFontAwesome.CartArrowDown` (:faicon:`cart-arrow-down`)
  * :const:`tlp.TulipFontAwesome.CartPlus` (:faicon:`cart-plus`)
  * :const:`tlp.TulipFontAwesome.Cc` (:faicon:`cc`)
  * :const:`tlp.TulipFontAwesome.CcAmex` (:faicon:`cc-amex`)
  * :const:`tlp.TulipFontAwesome.CcDinersClub` (:faicon:`cc-diners-club`)
  * :const:`tlp.TulipFontAwesome.CcDiscover` (:faicon:`cc-discover`)
  * :const:`tlp.TulipFontAwesome.CcJcb` (:faicon:`cc-jcb`)
  * :const:`tlp.TulipFontAwesome.CcMastercard` (:faicon:`cc-mastercard`)
  * :const:`tlp.TulipFontAwesome.CcPaypal` (:faicon:`cc-paypal`)
  * :const:`tlp.TulipFontAwesome.CcStripe` (:faicon:`cc-stripe`)
  * :const:`tlp.TulipFontAwesome.CcVisa` (:faicon:`cc-visa`)
  * :const:`tlp.TulipFontAwesome.Certificate` (:faicon:`certificate`)
  * :const:`tlp.TulipFontAwesome.Chain` (:faicon:`chain`)
  * :const:`tlp.TulipFontAwesome.ChainBroken` (:faicon:`chain-broken`)
  * :const:`tlp.TulipFontAwesome.Check` (:faicon:`check`)
  * :const:`tlp.TulipFontAwesome.CheckCircle` (:faicon:`check-circle`)
  * :const:`tlp.TulipFontAwesome.CheckCircleO` (:faicon:`check-circle-o`)
  * :const:`tlp.TulipFontAwesome.CheckSquare` (:faicon:`check-square`)
  * :const:`tlp.TulipFontAwesome.CheckSquareO` (:faicon:`check-square-o`)
  * :const:`tlp.TulipFontAwesome.ChevronCircleDown` (:faicon:`chevron-circle-down`)
  * :const:`tlp.TulipFontAwesome.ChevronCircleLeft` (:faicon:`chevron-circle-left`)
  * :const:`tlp.TulipFontAwesome.ChevronCircleRight` (:faicon:`chevron-circle-right`)
  * :const:`tlp.TulipFontAwesome.ChevronCircleUp` (:faicon:`chevron-circle-up`)
  * :const:`tlp.TulipFontAwesome.ChevronDown` (:faicon:`chevron-down`)
  * :const:`tlp.TulipFontAwesome.ChevronLeft` (:faicon:`chevron-left`)
  * :const:`tlp.TulipFontAwesome.ChevronRight` (:faicon:`chevron-right`)
  * :const:`tlp.TulipFontAwesome.ChevronUp` (:faicon:`chevron-up`)
  * :const:`tlp.TulipFontAwesome.Child` (:faicon:`child`)
  * :const:`tlp.TulipFontAwesome.Chrome` (:faicon:`chrome`)
  * :const:`tlp.TulipFontAwesome.Circle` (:faicon:`circle`)
  * :const:`tlp.TulipFontAwesome.CircleO` (:faicon:`circle-o`)
  * :const:`tlp.TulipFontAwesome.CircleONotch` (:faicon:`circle-o-notch`)
  * :const:`tlp.TulipFontAwesome.CircleThin` (:faicon:`circle-thin`)
  * :const:`tlp.TulipFontAwesome.Clipboard` (:faicon:`clipboard`)
  * :const:`tlp.TulipFontAwesome.ClockO` (:faicon:`clock-o`)
  * :const:`tlp.TulipFontAwesome.Clone` (:faicon:`clone`)
  * :const:`tlp.TulipFontAwesome.Close` (:faicon:`close`)
  * :const:`tlp.TulipFontAwesome.Cloud` (:faicon:`cloud`)
  * :const:`tlp.TulipFontAwesome.CloudDownload` (:faicon:`cloud-download`)
  * :const:`tlp.TulipFontAwesome.CloudUpload` (:faicon:`cloud-upload`)
  * :const:`tlp.TulipFontAwesome.Cny` (:faicon:`cny`)
  * :const:`tlp.TulipFontAwesome.Code` (:faicon:`code`)
  * :const:`tlp.TulipFontAwesome.CodeFork` (:faicon:`code-fork`)
  * :const:`tlp.TulipFontAwesome.Codepen` (:faicon:`codepen`)
  * :const:`tlp.TulipFontAwesome.Coffee` (:faicon:`coffee`)
  * :const:`tlp.TulipFontAwesome.Cog` (:faicon:`cog`)
  * :const:`tlp.TulipFontAwesome.Cogs` (:faicon:`cogs`)
  * :const:`tlp.TulipFontAwesome.Columns` (:faicon:`columns`)
  * :const:`tlp.TulipFontAwesome.Comment` (:faicon:`comment`)
  * :const:`tlp.TulipFontAwesome.CommentO` (:faicon:`comment-o`)
  * :const:`tlp.TulipFontAwesome.Commenting` (:faicon:`commenting`)
  * :const:`tlp.TulipFontAwesome.CommentingO` (:faicon:`commenting-o`)
  * :const:`tlp.TulipFontAwesome.Comments` (:faicon:`comments`)
  * :const:`tlp.TulipFontAwesome.CommentsO` (:faicon:`comments-o`)
  * :const:`tlp.TulipFontAwesome.Compass` (:faicon:`compass`)
  * :const:`tlp.TulipFontAwesome.Compress` (:faicon:`compress`)
  * :const:`tlp.TulipFontAwesome.Connectdevelop` (:faicon:`connectdevelop`)
  * :const:`tlp.TulipFontAwesome.Contao` (:faicon:`contao`)
  * :const:`tlp.TulipFontAwesome.Copy` (:faicon:`copy`)
  * :const:`tlp.TulipFontAwesome.Copyright` (:faicon:`copyright`)
  * :const:`tlp.TulipFontAwesome.CreativeCommons` (:faicon:`creative-commons`)
  * :const:`tlp.TulipFontAwesome.CreditCard` (:faicon:`credit-card`)
  * :const:`tlp.TulipFontAwesome.Crop` (:faicon:`crop`)
  * :const:`tlp.TulipFontAwesome.Crosshairs` (:faicon:`crosshairs`)
  * :const:`tlp.TulipFontAwesome.Css3` (:faicon:`css3`)
  * :const:`tlp.TulipFontAwesome.Cube` (:faicon:`cube`)
  * :const:`tlp.TulipFontAwesome.Cubes` (:faicon:`cubes`)
  * :const:`tlp.TulipFontAwesome.Cut` (:faicon:`cut`)
  * :const:`tlp.TulipFontAwesome.Cutlery` (:faicon:`cutlery`)
  * :const:`tlp.TulipFontAwesome.Dashboard` (:faicon:`dashboard`)
  * :const:`tlp.TulipFontAwesome.Dashcube` (:faicon:`dashcube`)
  * :const:`tlp.TulipFontAwesome.Database` (:faicon:`database`)
  * :const:`tlp.TulipFontAwesome.Dedent` (:faicon:`dedent`)
  * :const:`tlp.TulipFontAwesome.Delicious` (:faicon:`delicious`)
  * :const:`tlp.TulipFontAwesome.Desktop` (:faicon:`desktop`)
  * :const:`tlp.TulipFontAwesome.Deviantart` (:faicon:`deviantart`)
  * :const:`tlp.TulipFontAwesome.Diamond` (:faicon:`diamond`)
  * :const:`tlp.TulipFontAwesome.Digg` (:faicon:`digg`)
  * :const:`tlp.TulipFontAwesome.Dollar` (:faicon:`dollar`)
  * :const:`tlp.TulipFontAwesome.DotCircleO` (:faicon:`dot-circle-o`)
  * :const:`tlp.TulipFontAwesome.Download` (:faicon:`download`)
  * :const:`tlp.TulipFontAwesome.Dribbble` (:faicon:`dribbble`)
  * :const:`tlp.TulipFontAwesome.Dropbox` (:faicon:`dropbox`)
  * :const:`tlp.TulipFontAwesome.Drupal` (:faicon:`drupal`)
  * :const:`tlp.TulipFontAwesome.Edit` (:faicon:`edit`)
  * :const:`tlp.TulipFontAwesome.Eject` (:faicon:`eject`)
  * :const:`tlp.TulipFontAwesome.EllipsisH` (:faicon:`ellipsis-h`)
  * :const:`tlp.TulipFontAwesome.EllipsisV` (:faicon:`ellipsis-v`)
  * :const:`tlp.TulipFontAwesome.Empire` (:faicon:`empire`)
  * :const:`tlp.TulipFontAwesome.Envelope` (:faicon:`envelope`)
  * :const:`tlp.TulipFontAwesome.EnvelopeO` (:faicon:`envelope-o`)
  * :const:`tlp.TulipFontAwesome.EnvelopeSquare` (:faicon:`envelope-square`)
  * :const:`tlp.TulipFontAwesome.Eraser` (:faicon:`eraser`)
  * :const:`tlp.TulipFontAwesome.Eur` (:faicon:`eur`)
  * :const:`tlp.TulipFontAwesome.Euro` (:faicon:`euro`)
  * :const:`tlp.TulipFontAwesome.Exchange` (:faicon:`exchange`)
  * :const:`tlp.TulipFontAwesome.Exclamation` (:faicon:`exclamation`)
  * :const:`tlp.TulipFontAwesome.ExclamationCircle` (:faicon:`exclamation-circle`)
  * :const:`tlp.TulipFontAwesome.ExclamationTriangle` (:faicon:`exclamation-triangle`)
  * :const:`tlp.TulipFontAwesome.Expand` (:faicon:`expand`)
  * :const:`tlp.TulipFontAwesome.Expeditedssl` (:faicon:`expeditedssl`)
  * :const:`tlp.TulipFontAwesome.ExternalLink` (:faicon:`external-link`)
  * :const:`tlp.TulipFontAwesome.ExternalLinkSquare` (:faicon:`external-link-square`)
  * :const:`tlp.TulipFontAwesome.Eye` (:faicon:`eye`)
  * :const:`tlp.TulipFontAwesome.EyeSlash` (:faicon:`eye-slash`)
  * :const:`tlp.TulipFontAwesome.Eyedropper` (:faicon:`eyedropper`)
  * :const:`tlp.TulipFontAwesome.Facebook` (:faicon:`facebook`)
  * :const:`tlp.TulipFontAwesome.FacebookF` (:faicon:`facebook-f`)
  * :const:`tlp.TulipFontAwesome.FacebookOfficial` (:faicon:`facebook-official`)
  * :const:`tlp.TulipFontAwesome.FacebookSquare` (:faicon:`facebook-square`)
  * :const:`tlp.TulipFontAwesome.FastBackward` (:faicon:`fast-backward`)
  * :const:`tlp.TulipFontAwesome.FastForward` (:faicon:`fast-forward`)
  * :const:`tlp.TulipFontAwesome.Fax` (:faicon:`fax`)
  * :const:`tlp.TulipFontAwesome.Feed` (:faicon:`feed`)
  * :const:`tlp.TulipFontAwesome.Female` (:faicon:`female`)
  * :const:`tlp.TulipFontAwesome.FighterJet` (:faicon:`fighter-jet`)
  * :const:`tlp.TulipFontAwesome.File` (:faicon:`file`)
  * :const:`tlp.TulipFontAwesome.FileArchiveO` (:faicon:`file-archive-o`)
  * :const:`tlp.TulipFontAwesome.FileAudioO` (:faicon:`file-audio-o`)
  * :const:`tlp.TulipFontAwesome.FileCodeO` (:faicon:`file-code-o`)
  * :const:`tlp.TulipFontAwesome.FileExcelO` (:faicon:`file-excel-o`)
  * :const:`tlp.TulipFontAwesome.FileImageO` (:faicon:`file-image-o`)
  * :const:`tlp.TulipFontAwesome.FileMovieO` (:faicon:`file-movie-o`)
  * :const:`tlp.TulipFontAwesome.FileO` (:faicon:`file-o`)
  * :const:`tlp.TulipFontAwesome.FilePdfO` (:faicon:`file-pdf-o`)
  * :const:`tlp.TulipFontAwesome.FilePhotoO` (:faicon:`file-photo-o`)
  * :const:`tlp.TulipFontAwesome.FilePictureO` (:faicon:`file-picture-o`)
  * :const:`tlp.TulipFontAwesome.FilePowerpointO` (:faicon:`file-powerpoint-o`)
  * :const:`tlp.TulipFontAwesome.FileSoundO` (:faicon:`file-sound-o`)
  * :const:`tlp.TulipFontAwesome.FileText` (:faicon:`file-text`)
  * :const:`tlp.TulipFontAwesome.FileTextO` (:faicon:`file-text-o`)
  * :const:`tlp.TulipFontAwesome.FileVideoO` (:faicon:`file-video-o`)
  * :const:`tlp.TulipFontAwesome.FileWordO` (:faicon:`file-word-o`)
  * :const:`tlp.TulipFontAwesome.FileZipO` (:faicon:`file-zip-o`)
  * :const:`tlp.TulipFontAwesome.FilesO` (:faicon:`files-o`)
  * :const:`tlp.TulipFontAwesome.Film` (:faicon:`film`)
  * :const:`tlp.TulipFontAwesome.Filter` (:faicon:`filter`)
  * :const:`tlp.TulipFontAwesome.FireExtinguisher` (:faicon:`fire-extinguisher`)
  * :const:`tlp.TulipFontAwesome.Firefox` (:faicon:`firefox`)
  * :const:`tlp.TulipFontAwesome.Flag` (:faicon:`flag`)
  * :const:`tlp.TulipFontAwesome.FlagCheckered` (:faicon:`flag-checkered`)
  * :const:`tlp.TulipFontAwesome.FlagO` (:faicon:`flag-o`)
  * :const:`tlp.TulipFontAwesome.Flash` (:faicon:`flash`)
  * :const:`tlp.TulipFontAwesome.Flask` (:faicon:`flask`)
  * :const:`tlp.TulipFontAwesome.Flickr` (:faicon:`flickr`)
  * :const:`tlp.TulipFontAwesome.FloppyO` (:faicon:`floppy-o`)
  * :const:`tlp.TulipFontAwesome.Folder` (:faicon:`folder`)
  * :const:`tlp.TulipFontAwesome.FolderO` (:faicon:`folder-o`)
  * :const:`tlp.TulipFontAwesome.FolderOpen` (:faicon:`folder-open`)
  * :const:`tlp.TulipFontAwesome.FolderOpenO` (:faicon:`folder-open-o`)
  * :const:`tlp.TulipFontAwesome.Font` (:faicon:`font`)
  * :const:`tlp.TulipFontAwesome.Fonticons` (:faicon:`fonticons`)
  * :const:`tlp.TulipFontAwesome.Forumbee` (:faicon:`forumbee`)
  * :const:`tlp.TulipFontAwesome.Forward` (:faicon:`forward`)
  * :const:`tlp.TulipFontAwesome.Foursquare` (:faicon:`foursquare`)
  * :const:`tlp.TulipFontAwesome.FrownO` (:faicon:`frown-o`)
  * :const:`tlp.TulipFontAwesome.FutbolO` (:faicon:`futbol-o`)
  * :const:`tlp.TulipFontAwesome.Gamepad` (:faicon:`gamepad`)
  * :const:`tlp.TulipFontAwesome.Gavel` (:faicon:`gavel`)
  * :const:`tlp.TulipFontAwesome.Gbp` (:faicon:`gbp`)
  * :const:`tlp.TulipFontAwesome.Ge` (:faicon:`ge`)
  * :const:`tlp.TulipFontAwesome.Gear` (:faicon:`gear`)
  * :const:`tlp.TulipFontAwesome.Gears` (:faicon:`gears`)
  * :const:`tlp.TulipFontAwesome.Genderless` (:faicon:`genderless`)
  * :const:`tlp.TulipFontAwesome.GetPocket` (:faicon:`get-pocket`)
  * :const:`tlp.TulipFontAwesome.Gg` (:faicon:`gg`)
  * :const:`tlp.TulipFontAwesome.GgCircle` (:faicon:`gg-circle`)
  * :const:`tlp.TulipFontAwesome.Gift` (:faicon:`gift`)
  * :const:`tlp.TulipFontAwesome.Git` (:faicon:`git`)
  * :const:`tlp.TulipFontAwesome.GitSquare` (:faicon:`git-square`)
  * :const:`tlp.TulipFontAwesome.Github` (:faicon:`github`)
  * :const:`tlp.TulipFontAwesome.GithubAlt` (:faicon:`github-alt`)
  * :const:`tlp.TulipFontAwesome.GithubSquare` (:faicon:`github-square`)
  * :const:`tlp.TulipFontAwesome.Gittip` (:faicon:`gittip`)
  * :const:`tlp.TulipFontAwesome.Glass` (:faicon:`glass`)
  * :const:`tlp.TulipFontAwesome.Globe` (:faicon:`globe`)
  * :const:`tlp.TulipFontAwesome.Google` (:faicon:`google`)
  * :const:`tlp.TulipFontAwesome.GooglePlus` (:faicon:`google-plus`)
  * :const:`tlp.TulipFontAwesome.GooglePlusSquare` (:faicon:`google-plus-square`)
  * :const:`tlp.TulipFontAwesome.GoogleWallet` (:faicon:`google-wallet`)
  * :const:`tlp.TulipFontAwesome.GraduationCap` (:faicon:`graduation-cap`)
  * :const:`tlp.TulipFontAwesome.Gratipay` (:faicon:`gratipay`)
  * :const:`tlp.TulipFontAwesome.Group` (:faicon:`group`)
  * :const:`tlp.TulipFontAwesome.HSquare` (:faicon:`h-square`)
  * :const:`tlp.TulipFontAwesome.HackerNews` (:faicon:`hacker-news`)
  * :const:`tlp.TulipFontAwesome.HandGrabO` (:faicon:`hand-grab-o`)
  * :const:`tlp.TulipFontAwesome.HandLizardO` (:faicon:`hand-lizard-o`)
  * :const:`tlp.TulipFontAwesome.HandODown` (:faicon:`hand-o-down`)
  * :const:`tlp.TulipFontAwesome.HandOLeft` (:faicon:`hand-o-left`)
  * :const:`tlp.TulipFontAwesome.HandORight` (:faicon:`hand-o-right`)
  * :const:`tlp.TulipFontAwesome.HandOUp` (:faicon:`hand-o-up`)
  * :const:`tlp.TulipFontAwesome.HandPaperO` (:faicon:`hand-paper-o`)
  * :const:`tlp.TulipFontAwesome.HandPeaceO` (:faicon:`hand-peace-o`)
  * :const:`tlp.TulipFontAwesome.HandPointerO` (:faicon:`hand-pointer-o`)
  * :const:`tlp.TulipFontAwesome.HandRockO` (:faicon:`hand-rock-o`)
  * :const:`tlp.TulipFontAwesome.HandScissorsO` (:faicon:`hand-scissors-o`)
  * :const:`tlp.TulipFontAwesome.HandSpockO` (:faicon:`hand-spock-o`)
  * :const:`tlp.TulipFontAwesome.HandStopO` (:faicon:`hand-stop-o`)
  * :const:`tlp.TulipFontAwesome.HddO` (:faicon:`hdd-o`)
  * :const:`tlp.TulipFontAwesome.Header` (:faicon:`header`)
  * :const:`tlp.TulipFontAwesome.Headphones` (:faicon:`headphones`)
  * :const:`tlp.TulipFontAwesome.Heart` (:faicon:`heart`)
  * :const:`tlp.TulipFontAwesome.HeartO` (:faicon:`heart-o`)
  * :const:`tlp.TulipFontAwesome.Heartbeat` (:faicon:`heartbeat`)
  * :const:`tlp.TulipFontAwesome.History` (:faicon:`history`)
  * :const:`tlp.TulipFontAwesome.Home` (:faicon:`home`)
  * :const:`tlp.TulipFontAwesome.HospitalO` (:faicon:`hospital-o`)
  * :const:`tlp.TulipFontAwesome.Hotel` (:faicon:`hotel`)
  * :const:`tlp.TulipFontAwesome.Hourglass` (:faicon:`hourglass`)
  * :const:`tlp.TulipFontAwesome.Hourglass1` (:faicon:`hourglass-1`)
  * :const:`tlp.TulipFontAwesome.Hourglass2` (:faicon:`hourglass-2`)
  * :const:`tlp.TulipFontAwesome.Hourglass3` (:faicon:`hourglass-3`)
  * :const:`tlp.TulipFontAwesome.HourglassEnd` (:faicon:`hourglass-end`)
  * :const:`tlp.TulipFontAwesome.HourglassHalf` (:faicon:`hourglass-half`)
  * :const:`tlp.TulipFontAwesome.HourglassO` (:faicon:`hourglass-o`)
  * :const:`tlp.TulipFontAwesome.HourglassStart` (:faicon:`hourglass-start`)
  * :const:`tlp.TulipFontAwesome.Houzz` (:faicon:`houzz`)
  * :const:`tlp.TulipFontAwesome.Html5` (:faicon:`html5`)
  * :const:`tlp.TulipFontAwesome.ICursor` (:faicon:`i-cursor`)
  * :const:`tlp.TulipFontAwesome.Ils` (:faicon:`ils`)
  * :const:`tlp.TulipFontAwesome.Image` (:faicon:`image`)
  * :const:`tlp.TulipFontAwesome.Inbox` (:faicon:`inbox`)
  * :const:`tlp.TulipFontAwesome.Indent` (:faicon:`indent`)
  * :const:`tlp.TulipFontAwesome.Industry` (:faicon:`industry`)
  * :const:`tlp.TulipFontAwesome.Info` (:faicon:`info`)
  * :const:`tlp.TulipFontAwesome.InfoCircle` (:faicon:`info-circle`)
  * :const:`tlp.TulipFontAwesome.Inr` (:faicon:`inr`)
  * :const:`tlp.TulipFontAwesome.Instagram` (:faicon:`instagram`)
  * :const:`tlp.TulipFontAwesome.Institution` (:faicon:`institution`)
  * :const:`tlp.TulipFontAwesome.InternetExplorer` (:faicon:`internet-explorer`)
  * :const:`tlp.TulipFontAwesome.Intersex` (:faicon:`intersex`)
  * :const:`tlp.TulipFontAwesome.Ioxhost` (:faicon:`ioxhost`)
  * :const:`tlp.TulipFontAwesome.Italic` (:faicon:`italic`)
  * :const:`tlp.TulipFontAwesome.Joomla` (:faicon:`joomla`)
  * :const:`tlp.TulipFontAwesome.Jpy` (:faicon:`jpy`)
  * :const:`tlp.TulipFontAwesome.Jsfiddle` (:faicon:`jsfiddle`)
  * :const:`tlp.TulipFontAwesome.Key` (:faicon:`key`)
  * :const:`tlp.TulipFontAwesome.KeyboardO` (:faicon:`keyboard-o`)
  * :const:`tlp.TulipFontAwesome.Krw` (:faicon:`krw`)
  * :const:`tlp.TulipFontAwesome.Language` (:faicon:`language`)
  * :const:`tlp.TulipFontAwesome.Laptop` (:faicon:`laptop`)
  * :const:`tlp.TulipFontAwesome.Lastfm` (:faicon:`lastfm`)
  * :const:`tlp.TulipFontAwesome.LastfmSquare` (:faicon:`lastfm-square`)
  * :const:`tlp.TulipFontAwesome.Leaf` (:faicon:`leaf`)
  * :const:`tlp.TulipFontAwesome.Leanpub` (:faicon:`leanpub`)
  * :const:`tlp.TulipFontAwesome.Legal` (:faicon:`legal`)
  * :const:`tlp.TulipFontAwesome.LemonO` (:faicon:`lemon-o`)
  * :const:`tlp.TulipFontAwesome.LevelDown` (:faicon:`level-down`)
  * :const:`tlp.TulipFontAwesome.LevelUp` (:faicon:`level-up`)
  * :const:`tlp.TulipFontAwesome.LifeBouy` (:faicon:`life-bouy`)
  * :const:`tlp.TulipFontAwesome.LifeBuoy` (:faicon:`life-buoy`)
  * :const:`tlp.TulipFontAwesome.LifeRing` (:faicon:`life-ring`)
  * :const:`tlp.TulipFontAwesome.LifeSaver` (:faicon:`life-saver`)
  * :const:`tlp.TulipFontAwesome.LightbulbO` (:faicon:`lightbulb-o`)
  * :const:`tlp.TulipFontAwesome.LineChart` (:faicon:`line-chart`)
  * :const:`tlp.TulipFontAwesome.Link` (:faicon:`link`)
  * :const:`tlp.TulipFontAwesome.Linkedin` (:faicon:`linkedin`)
  * :const:`tlp.TulipFontAwesome.LinkedinSquare` (:faicon:`linkedin-square`)
  * :const:`tlp.TulipFontAwesome.Linux` (:faicon:`linux`)
  * :const:`tlp.TulipFontAwesome.List` (:faicon:`list`)
  * :const:`tlp.TulipFontAwesome.ListAlt` (:faicon:`list-alt`)
  * :const:`tlp.TulipFontAwesome.ListOl` (:faicon:`list-ol`)
  * :const:`tlp.TulipFontAwesome.ListUl` (:faicon:`list-ul`)
  * :const:`tlp.TulipFontAwesome.LocationArrow` (:faicon:`location-arrow`)
  * :const:`tlp.TulipFontAwesome.Lock` (:faicon:`lock`)
  * :const:`tlp.TulipFontAwesome.LongArrowDown` (:faicon:`long-arrow-down`)
  * :const:`tlp.TulipFontAwesome.LongArrowLeft` (:faicon:`long-arrow-left`)
  * :const:`tlp.TulipFontAwesome.LongArrowRight` (:faicon:`long-arrow-right`)
  * :const:`tlp.TulipFontAwesome.LongArrowUp` (:faicon:`long-arrow-up`)
  * :const:`tlp.TulipFontAwesome.Magic` (:faicon:`magic`)
  * :const:`tlp.TulipFontAwesome.Magnet` (:faicon:`magnet`)
  * :const:`tlp.TulipFontAwesome.MailForward` (:faicon:`mail-forward`)
  * :const:`tlp.TulipFontAwesome.MailReply` (:faicon:`mail-reply`)
  * :const:`tlp.TulipFontAwesome.MailReplyAll` (:faicon:`mail-reply-all`)
  * :const:`tlp.TulipFontAwesome.Male` (:faicon:`male`)
  * :const:`tlp.TulipFontAwesome.Map` (:faicon:`map`)
  * :const:`tlp.TulipFontAwesome.MapMarker` (:faicon:`map-marker`)
  * :const:`tlp.TulipFontAwesome.MapO` (:faicon:`map-o`)
  * :const:`tlp.TulipFontAwesome.MapPin` (:faicon:`map-pin`)
  * :const:`tlp.TulipFontAwesome.MapSigns` (:faicon:`map-signs`)
  * :const:`tlp.TulipFontAwesome.Mars` (:faicon:`mars`)
  * :const:`tlp.TulipFontAwesome.MarsDouble` (:faicon:`mars-double`)
  * :const:`tlp.TulipFontAwesome.MarsStroke` (:faicon:`mars-stroke`)
  * :const:`tlp.TulipFontAwesome.MarsStrokeH` (:faicon:`mars-stroke-h`)
  * :const:`tlp.TulipFontAwesome.MarsStrokeV` (:faicon:`mars-stroke-v`)
  * :const:`tlp.TulipFontAwesome.Maxcdn` (:faicon:`maxcdn`)
  * :const:`tlp.TulipFontAwesome.Meanpath` (:faicon:`meanpath`)
  * :const:`tlp.TulipFontAwesome.Medium` (:faicon:`medium`)
  * :const:`tlp.TulipFontAwesome.Medkit` (:faicon:`medkit`)
  * :const:`tlp.TulipFontAwesome.MehO` (:faicon:`meh-o`)
  * :const:`tlp.TulipFontAwesome.Mercury` (:faicon:`mercury`)
  * :const:`tlp.TulipFontAwesome.Microphone` (:faicon:`microphone`)
  * :const:`tlp.TulipFontAwesome.MicrophoneSlash` (:faicon:`microphone-slash`)
  * :const:`tlp.TulipFontAwesome.Minus` (:faicon:`minus`)
  * :const:`tlp.TulipFontAwesome.MinusCircle` (:faicon:`minus-circle`)
  * :const:`tlp.TulipFontAwesome.MinusSquare` (:faicon:`minus-square`)
  * :const:`tlp.TulipFontAwesome.MinusSquareO` (:faicon:`minus-square-o`)
  * :const:`tlp.TulipFontAwesome.Mobile` (:faicon:`mobile`)
  * :const:`tlp.TulipFontAwesome.MobilePhone` (:faicon:`mobile-phone`)
  * :const:`tlp.TulipFontAwesome.Money` (:faicon:`money`)
  * :const:`tlp.TulipFontAwesome.MoonO` (:faicon:`moon-o`)
  * :const:`tlp.TulipFontAwesome.MortarBoard` (:faicon:`mortar-board`)
  * :const:`tlp.TulipFontAwesome.Motorcycle` (:faicon:`motorcycle`)
  * :const:`tlp.TulipFontAwesome.MousePointer` (:faicon:`mouse-pointer`)
  * :const:`tlp.TulipFontAwesome.Music` (:faicon:`music`)
  * :const:`tlp.TulipFontAwesome.Navicon` (:faicon:`navicon`)
  * :const:`tlp.TulipFontAwesome.Neuter` (:faicon:`neuter`)
  * :const:`tlp.TulipFontAwesome.NewspaperO` (:faicon:`newspaper-o`)
  * :const:`tlp.TulipFontAwesome.ObjectGroup` (:faicon:`object-group`)
  * :const:`tlp.TulipFontAwesome.ObjectUngroup` (:faicon:`object-ungroup`)
  * :const:`tlp.TulipFontAwesome.Odnoklassniki` (:faicon:`odnoklassniki`)
  * :const:`tlp.TulipFontAwesome.OdnoklassnikiSquare` (:faicon:`odnoklassniki-square`)
  * :const:`tlp.TulipFontAwesome.Opencart` (:faicon:`opencart`)
  * :const:`tlp.TulipFontAwesome.Openid` (:faicon:`openid`)
  * :const:`tlp.TulipFontAwesome.Opera` (:faicon:`opera`)
  * :const:`tlp.TulipFontAwesome.Outdent` (:faicon:`outdent`)
  * :const:`tlp.TulipFontAwesome.Pagelines` (:faicon:`pagelines`)
  * :const:`tlp.TulipFontAwesome.PaintBrush` (:faicon:`paint-brush`)
  * :const:`tlp.TulipFontAwesome.PaperPlane` (:faicon:`paper-plane`)
  * :const:`tlp.TulipFontAwesome.PaperPlaneO` (:faicon:`paper-plane-o`)
  * :const:`tlp.TulipFontAwesome.Paperclip` (:faicon:`paperclip`)
  * :const:`tlp.TulipFontAwesome.Paragraph` (:faicon:`paragraph`)
  * :const:`tlp.TulipFontAwesome.Paste` (:faicon:`paste`)
  * :const:`tlp.TulipFontAwesome.Pause` (:faicon:`pause`)
  * :const:`tlp.TulipFontAwesome.Paw` (:faicon:`paw`)
  * :const:`tlp.TulipFontAwesome.Paypal` (:faicon:`paypal`)
  * :const:`tlp.TulipFontAwesome.Pencil` (:faicon:`pencil`)
  * :const:`tlp.TulipFontAwesome.PencilSquare` (:faicon:`pencil-square`)
  * :const:`tlp.TulipFontAwesome.PencilSquareO` (:faicon:`pencil-square-o`)
  * :const:`tlp.TulipFontAwesome.Phone` (:faicon:`phone`)
  * :const:`tlp.TulipFontAwesome.PhoneSquare` (:faicon:`phone-square`)
  * :const:`tlp.TulipFontAwesome.Photo` (:faicon:`photo`)
  * :const:`tlp.TulipFontAwesome.PictureO` (:faicon:`picture-o`)
  * :const:`tlp.TulipFontAwesome.PieChart` (:faicon:`pie-chart`)
  * :const:`tlp.TulipFontAwesome.PiedPiper` (:faicon:`pied-piper`)
  * :const:`tlp.TulipFontAwesome.PiedPiperAlt` (:faicon:`pied-piper-alt`)
  * :const:`tlp.TulipFontAwesome.Pinterest` (:faicon:`pinterest`)
  * :const:`tlp.TulipFontAwesome.PinterestP` (:faicon:`pinterest-p`)
  * :const:`tlp.TulipFontAwesome.PinterestSquare` (:faicon:`pinterest-square`)
  * :const:`tlp.TulipFontAwesome.Plane` (:faicon:`plane`)
  * :const:`tlp.TulipFontAwesome.Play` (:faicon:`play`)
  * :const:`tlp.TulipFontAwesome.PlayCircle` (:faicon:`play-circle`)
  * :const:`tlp.TulipFontAwesome.PlayCircleO` (:faicon:`play-circle-o`)
  * :const:`tlp.TulipFontAwesome.Plug` (:faicon:`plug`)
  * :const:`tlp.TulipFontAwesome.Plus` (:faicon:`plus`)
  * :const:`tlp.TulipFontAwesome.PlusCircle` (:faicon:`plus-circle`)
  * :const:`tlp.TulipFontAwesome.PlusSquare` (:faicon:`plus-square`)
  * :const:`tlp.TulipFontAwesome.PlusSquareO` (:faicon:`plus-square-o`)
  * :const:`tlp.TulipFontAwesome.PowerOff` (:faicon:`power-off`)
  * :const:`tlp.TulipFontAwesome.Print` (:faicon:`print`)
  * :const:`tlp.TulipFontAwesome.PuzzlePiece` (:faicon:`puzzle-piece`)
  * :const:`tlp.TulipFontAwesome.Qq` (:faicon:`qq`)
  * :const:`tlp.TulipFontAwesome.Qrcode` (:faicon:`qrcode`)
  * :const:`tlp.TulipFontAwesome.Question` (:faicon:`question`)
  * :const:`tlp.TulipFontAwesome.QuestionCircle` (:faicon:`question-circle`)
  * :const:`tlp.TulipFontAwesome.QuoteLeft` (:faicon:`quote-left`)
  * :const:`tlp.TulipFontAwesome.QuoteRight` (:faicon:`quote-right`)
  * :const:`tlp.TulipFontAwesome.Ra` (:faicon:`ra`)
  * :const:`tlp.TulipFontAwesome.Random` (:faicon:`random`)
  * :const:`tlp.TulipFontAwesome.Rebel` (:faicon:`rebel`)
  * :const:`tlp.TulipFontAwesome.Recycle` (:faicon:`recycle`)
  * :const:`tlp.TulipFontAwesome.Reddit` (:faicon:`reddit`)
  * :const:`tlp.TulipFontAwesome.RedditSquare` (:faicon:`reddit-square`)
  * :const:`tlp.TulipFontAwesome.Refresh` (:faicon:`refresh`)
  * :const:`tlp.TulipFontAwesome.Registered` (:faicon:`registered`)
  * :const:`tlp.TulipFontAwesome.Remove` (:faicon:`remove`)
  * :const:`tlp.TulipFontAwesome.Renren` (:faicon:`renren`)
  * :const:`tlp.TulipFontAwesome.Reorder` (:faicon:`reorder`)
  * :const:`tlp.TulipFontAwesome.Repeat` (:faicon:`repeat`)
  * :const:`tlp.TulipFontAwesome.Reply` (:faicon:`reply`)
  * :const:`tlp.TulipFontAwesome.ReplyAll` (:faicon:`reply-all`)
  * :const:`tlp.TulipFontAwesome.Retweet` (:faicon:`retweet`)
  * :const:`tlp.TulipFontAwesome.Rmb` (:faicon:`rmb`)
  * :const:`tlp.TulipFontAwesome.Road` (:faicon:`road`)
  * :const:`tlp.TulipFontAwesome.Rocket` (:faicon:`rocket`)
  * :const:`tlp.TulipFontAwesome.RotateLeft` (:faicon:`rotate-left`)
  * :const:`tlp.TulipFontAwesome.RotateRight` (:faicon:`rotate-right`)
  * :const:`tlp.TulipFontAwesome.Rouble` (:faicon:`rouble`)
  * :const:`tlp.TulipFontAwesome.Rss` (:faicon:`rss`)
  * :const:`tlp.TulipFontAwesome.RssSquare` (:faicon:`rss-square`)
  * :const:`tlp.TulipFontAwesome.Rub` (:faicon:`rub`)
  * :const:`tlp.TulipFontAwesome.Ruble` (:faicon:`ruble`)
  * :const:`tlp.TulipFontAwesome.Rupee` (:faicon:`rupee`)
  * :const:`tlp.TulipFontAwesome.Safari` (:faicon:`safari`)
  * :const:`tlp.TulipFontAwesome.Save` (:faicon:`save`)
  * :const:`tlp.TulipFontAwesome.Scissors` (:faicon:`scissors`)
  * :const:`tlp.TulipFontAwesome.Search` (:faicon:`search`)
  * :const:`tlp.TulipFontAwesome.SearchMinus` (:faicon:`search-minus`)
  * :const:`tlp.TulipFontAwesome.SearchPlus` (:faicon:`search-plus`)
  * :const:`tlp.TulipFontAwesome.Sellsy` (:faicon:`sellsy`)
  * :const:`tlp.TulipFontAwesome.Send` (:faicon:`send`)
  * :const:`tlp.TulipFontAwesome.SendO` (:faicon:`send-o`)
  * :const:`tlp.TulipFontAwesome.Server` (:faicon:`server`)
  * :const:`tlp.TulipFontAwesome.Share` (:faicon:`share`)
  * :const:`tlp.TulipFontAwesome.ShareAlt` (:faicon:`share-alt`)
  * :const:`tlp.TulipFontAwesome.ShareAltSquare` (:faicon:`share-alt-square`)
  * :const:`tlp.TulipFontAwesome.ShareSquare` (:faicon:`share-square`)
  * :const:`tlp.TulipFontAwesome.ShareSquareO` (:faicon:`share-square-o`)
  * :const:`tlp.TulipFontAwesome.Shekel` (:faicon:`shekel`)
  * :const:`tlp.TulipFontAwesome.Sheqel` (:faicon:`sheqel`)
  * :const:`tlp.TulipFontAwesome.Shield` (:faicon:`shield`)
  * :const:`tlp.TulipFontAwesome.Ship` (:faicon:`ship`)
  * :const:`tlp.TulipFontAwesome.Shirtsinbulk` (:faicon:`shirtsinbulk`)
  * :const:`tlp.TulipFontAwesome.ShoppingCart` (:faicon:`shopping-cart`)
  * :const:`tlp.TulipFontAwesome.SignIn` (:faicon:`sign-in`)
  * :const:`tlp.TulipFontAwesome.SignOut` (:faicon:`sign-out`)
  * :const:`tlp.TulipFontAwesome.Signal` (:faicon:`signal`)
  * :const:`tlp.TulipFontAwesome.Simplybuilt` (:faicon:`simplybuilt`)
  * :const:`tlp.TulipFontAwesome.Sitemap` (:faicon:`sitemap`)
  * :const:`tlp.TulipFontAwesome.Skyatlas` (:faicon:`skyatlas`)
  * :const:`tlp.TulipFontAwesome.Skype` (:faicon:`skype`)
  * :const:`tlp.TulipFontAwesome.Slack` (:faicon:`slack`)
  * :const:`tlp.TulipFontAwesome.Sliders` (:faicon:`sliders`)
  * :const:`tlp.TulipFontAwesome.Slideshare` (:faicon:`slideshare`)
  * :const:`tlp.TulipFontAwesome.SmileO` (:faicon:`smile-o`)
  * :const:`tlp.TulipFontAwesome.SoccerBallO` (:faicon:`soccer-ball-o`)
  * :const:`tlp.TulipFontAwesome.Sort` (:faicon:`sort`)
  * :const:`tlp.TulipFontAwesome.SortAlphaAsc` (:faicon:`sort-alpha-asc`)
  * :const:`tlp.TulipFontAwesome.SortAlphaDesc` (:faicon:`sort-alpha-desc`)
  * :const:`tlp.TulipFontAwesome.SortAmountAsc` (:faicon:`sort-amount-asc`)
  * :const:`tlp.TulipFontAwesome.SortAmountDesc` (:faicon:`sort-amount-desc`)
  * :const:`tlp.TulipFontAwesome.SortAsc` (:faicon:`sort-asc`)
  * :const:`tlp.TulipFontAwesome.SortDesc` (:faicon:`sort-desc`)
  * :const:`tlp.TulipFontAwesome.SortDown` (:faicon:`sort-down`)
  * :const:`tlp.TulipFontAwesome.SortNumericAsc` (:faicon:`sort-numeric-asc`)
  * :const:`tlp.TulipFontAwesome.SortNumericDesc` (:faicon:`sort-numeric-desc`)
  * :const:`tlp.TulipFontAwesome.SortUp` (:faicon:`sort-up`)
  * :const:`tlp.TulipFontAwesome.Soundcloud` (:faicon:`soundcloud`)
  * :const:`tlp.TulipFontAwesome.SpaceShuttle` (:faicon:`space-shuttle`)
  * :const:`tlp.TulipFontAwesome.Spinner` (:faicon:`spinner`)
  * :const:`tlp.TulipFontAwesome.Spoon` (:faicon:`spoon`)
  * :const:`tlp.TulipFontAwesome.Spotify` (:faicon:`spotify`)
  * :const:`tlp.TulipFontAwesome.Square` (:faicon:`square`)
  * :const:`tlp.TulipFontAwesome.SquareO` (:faicon:`square-o`)
  * :const:`tlp.TulipFontAwesome.StackExchange` (:faicon:`stack-exchange`)
  * :const:`tlp.TulipFontAwesome.StackOverflow` (:faicon:`stack-overflow`)
  * :const:`tlp.TulipFontAwesome.Star` (:faicon:`star`)
  * :const:`tlp.TulipFontAwesome.StarHalf` (:faicon:`star-half`)
  * :const:`tlp.TulipFontAwesome.StarHalfEmpty` (:faicon:`star-half-empty`)
  * :const:`tlp.TulipFontAwesome.StarHalfFull` (:faicon:`star-half-full`)
  * :const:`tlp.TulipFontAwesome.StarHalfO` (:faicon:`star-half-o`)
  * :const:`tlp.TulipFontAwesome.StarO` (:faicon:`star-o`)
  * :const:`tlp.TulipFontAwesome.Steam` (:faicon:`steam`)
  * :const:`tlp.TulipFontAwesome.SteamSquare` (:faicon:`steam-square`)
  * :const:`tlp.TulipFontAwesome.StepBackward` (:faicon:`step-backward`)
  * :const:`tlp.TulipFontAwesome.StepForward` (:faicon:`step-forward`)
  * :const:`tlp.TulipFontAwesome.Stethoscope` (:faicon:`stethoscope`)
  * :const:`tlp.TulipFontAwesome.StickyNote` (:faicon:`sticky-note`)
  * :const:`tlp.TulipFontAwesome.StickyNoteO` (:faicon:`sticky-note-o`)
  * :const:`tlp.TulipFontAwesome.Stop` (:faicon:`stop`)
  * :const:`tlp.TulipFontAwesome.StreetView` (:faicon:`street-view`)
  * :const:`tlp.TulipFontAwesome.Strikethrough` (:faicon:`strikethrough`)
  * :const:`tlp.TulipFontAwesome.Stumbleupon` (:faicon:`stumbleupon`)
  * :const:`tlp.TulipFontAwesome.StumbleuponCircle` (:faicon:`stumbleupon-circle`)
  * :const:`tlp.TulipFontAwesome.Subscript` (:faicon:`subscript`)
  * :const:`tlp.TulipFontAwesome.Subway` (:faicon:`subway`)
  * :const:`tlp.TulipFontAwesome.Suitcase` (:faicon:`suitcase`)
  * :const:`tlp.TulipFontAwesome.SunO` (:faicon:`sun-o`)
  * :const:`tlp.TulipFontAwesome.Superscript` (:faicon:`superscript`)
  * :const:`tlp.TulipFontAwesome.Support` (:faicon:`support`)
  * :const:`tlp.TulipFontAwesome.Table` (:faicon:`table`)
  * :const:`tlp.TulipFontAwesome.Tablet` (:faicon:`tablet`)
  * :const:`tlp.TulipFontAwesome.Tachometer` (:faicon:`tachometer`)
  * :const:`tlp.TulipFontAwesome.Tag` (:faicon:`tag`)
  * :const:`tlp.TulipFontAwesome.Tags` (:faicon:`tags`)
  * :const:`tlp.TulipFontAwesome.Tasks` (:faicon:`tasks`)
  * :const:`tlp.TulipFontAwesome.Taxi` (:faicon:`taxi`)
  * :const:`tlp.TulipFontAwesome.Television` (:faicon:`television`)
  * :const:`tlp.TulipFontAwesome.TencentWeibo` (:faicon:`tencent-weibo`)
  * :const:`tlp.TulipFontAwesome.Terminal` (:faicon:`terminal`)
  * :const:`tlp.TulipFontAwesome.TextHeight` (:faicon:`text-height`)
  * :const:`tlp.TulipFontAwesome.TextWidth` (:faicon:`text-width`)
  * :const:`tlp.TulipFontAwesome.Th` (:faicon:`th`)
  * :const:`tlp.TulipFontAwesome.ThLarge` (:faicon:`th-large`)
  * :const:`tlp.TulipFontAwesome.ThList` (:faicon:`th-list`)
  * :const:`tlp.TulipFontAwesome.ThumbTack` (:faicon:`thumb-tack`)
  * :const:`tlp.TulipFontAwesome.ThumbsDown` (:faicon:`thumbs-down`)
  * :const:`tlp.TulipFontAwesome.ThumbsODown` (:faicon:`thumbs-o-down`)
  * :const:`tlp.TulipFontAwesome.ThumbsOUp` (:faicon:`thumbs-o-up`)
  * :const:`tlp.TulipFontAwesome.ThumbsUp` (:faicon:`thumbs-up`)
  * :const:`tlp.TulipFontAwesome.Ticket` (:faicon:`ticket`)
  * :const:`tlp.TulipFontAwesome.Times` (:faicon:`times`)
  * :const:`tlp.TulipFontAwesome.TimesCircle` (:faicon:`times-circle`)
  * :const:`tlp.TulipFontAwesome.TimesCircleO` (:faicon:`times-circle-o`)
  * :const:`tlp.TulipFontAwesome.Tint` (:faicon:`tint`)
  * :const:`tlp.TulipFontAwesome.ToggleDown` (:faicon:`toggle-down`)
  * :const:`tlp.TulipFontAwesome.ToggleLeft` (:faicon:`toggle-left`)
  * :const:`tlp.TulipFontAwesome.ToggleOff` (:faicon:`toggle-off`)
  * :const:`tlp.TulipFontAwesome.ToggleOn` (:faicon:`toggle-on`)
  * :const:`tlp.TulipFontAwesome.ToggleRight` (:faicon:`toggle-right`)
  * :const:`tlp.TulipFontAwesome.ToggleUp` (:faicon:`toggle-up`)
  * :const:`tlp.TulipFontAwesome.Trademark` (:faicon:`trademark`)
  * :const:`tlp.TulipFontAwesome.Train` (:faicon:`train`)
  * :const:`tlp.TulipFontAwesome.Transgender` (:faicon:`transgender`)
  * :const:`tlp.TulipFontAwesome.TransgenderAlt` (:faicon:`transgender-alt`)
  * :const:`tlp.TulipFontAwesome.Trash` (:faicon:`trash`)
  * :const:`tlp.TulipFontAwesome.TrashO` (:faicon:`trash-o`)
  * :const:`tlp.TulipFontAwesome.Tree` (:faicon:`tree`)
  * :const:`tlp.TulipFontAwesome.Trello` (:faicon:`trello`)
  * :const:`tlp.TulipFontAwesome.Tripadvisor` (:faicon:`tripadvisor`)
  * :const:`tlp.TulipFontAwesome.Trophy` (:faicon:`trophy`)
  * :const:`tlp.TulipFontAwesome.Truck` (:faicon:`truck`)
  * :const:`tlp.TulipFontAwesome.Try` (:faicon:`try`)
  * :const:`tlp.TulipFontAwesome.Tty` (:faicon:`tty`)
  * :const:`tlp.TulipFontAwesome.Tumblr` (:faicon:`tumblr`)
  * :const:`tlp.TulipFontAwesome.TumblrSquare` (:faicon:`tumblr-square`)
  * :const:`tlp.TulipFontAwesome.TurkishLira` (:faicon:`turkish-lira`)
  * :const:`tlp.TulipFontAwesome.Tv` (:faicon:`tv`)
  * :const:`tlp.TulipFontAwesome.Twitch` (:faicon:`twitch`)
  * :const:`tlp.TulipFontAwesome.Twitter` (:faicon:`twitter`)
  * :const:`tlp.TulipFontAwesome.TwitterSquare` (:faicon:`twitter-square`)
  * :const:`tlp.TulipFontAwesome.Umbrella` (:faicon:`umbrella`)
  * :const:`tlp.TulipFontAwesome.Underline` (:faicon:`underline`)
  * :const:`tlp.TulipFontAwesome.Undo` (:faicon:`undo`)
  * :const:`tlp.TulipFontAwesome.University` (:faicon:`university`)
  * :const:`tlp.TulipFontAwesome.Unlink` (:faicon:`unlink`)
  * :const:`tlp.TulipFontAwesome.Unlock` (:faicon:`unlock`)
  * :const:`tlp.TulipFontAwesome.UnlockAlt` (:faicon:`unlock-alt`)
  * :const:`tlp.TulipFontAwesome.Unsorted` (:faicon:`unsorted`)
  * :const:`tlp.TulipFontAwesome.Upload` (:faicon:`upload`)
  * :const:`tlp.TulipFontAwesome.Usd` (:faicon:`usd`)
  * :const:`tlp.TulipFontAwesome.User` (:faicon:`user`)
  * :const:`tlp.TulipFontAwesome.UserMd` (:faicon:`user-md`)
  * :const:`tlp.TulipFontAwesome.UserPlus` (:faicon:`user-plus`)
  * :const:`tlp.TulipFontAwesome.UserSecret` (:faicon:`user-secret`)
  * :const:`tlp.TulipFontAwesome.UserTimes` (:faicon:`user-times`)
  * :const:`tlp.TulipFontAwesome.Users` (:faicon:`users`)
  * :const:`tlp.TulipFontAwesome.Venus` (:faicon:`venus`)
  * :const:`tlp.TulipFontAwesome.VenusDouble` (:faicon:`venus-double`)
  * :const:`tlp.TulipFontAwesome.VenusMars` (:faicon:`venus-mars`)
  * :const:`tlp.TulipFontAwesome.Viacoin` (:faicon:`viacoin`)
  * :const:`tlp.TulipFontAwesome.VideoCamera` (:faicon:`video-camera`)
  * :const:`tlp.TulipFontAwesome.Vimeo` (:faicon:`vimeo`)
  * :const:`tlp.TulipFontAwesome.VimeoSquare` (:faicon:`vimeo-square`)
  * :const:`tlp.TulipFontAwesome.Vine` (:faicon:`vine`)
  * :const:`tlp.TulipFontAwesome.Vk` (:faicon:`vk`)
  * :const:`tlp.TulipFontAwesome.VolumeDown` (:faicon:`volume-down`)
  * :const:`tlp.TulipFontAwesome.VolumeOff` (:faicon:`volume-off`)
  * :const:`tlp.TulipFontAwesome.VolumeUp` (:faicon:`volume-up`)
  * :const:`tlp.TulipFontAwesome.Warning` (:faicon:`warning`)
  * :const:`tlp.TulipFontAwesome.Wechat` (:faicon:`wechat`)
  * :const:`tlp.TulipFontAwesome.Weibo` (:faicon:`weibo`)
  * :const:`tlp.TulipFontAwesome.Weixin` (:faicon:`weixin`)
  * :const:`tlp.TulipFontAwesome.Whatsapp` (:faicon:`whatsapp`)
  * :const:`tlp.TulipFontAwesome.Wheelchair` (:faicon:`wheelchair`)
  * :const:`tlp.TulipFontAwesome.Wifi` (:faicon:`wifi`)
  * :const:`tlp.TulipFontAwesome.WikipediaW` (:faicon:`wikipedia-w`)
  * :const:`tlp.TulipFontAwesome.Windows` (:faicon:`windows`)
  * :const:`tlp.TulipFontAwesome.Won` (:faicon:`won`)
  * :const:`tlp.TulipFontAwesome.Wordpress` (:faicon:`wordpress`)
  * :const:`tlp.TulipFontAwesome.Wrench` (:faicon:`wrench`)
  * :const:`tlp.TulipFontAwesome.Xing` (:faicon:`xing`)
  * :const:`tlp.TulipFontAwesome.XingSquare` (:faicon:`xing-square`)
  * :const:`tlp.TulipFontAwesome.YCombinator` (:faicon:`y-combinator`)
  * :const:`tlp.TulipFontAwesome.YCombinatorSquare` (:faicon:`y-combinator-square`)
  * :const:`tlp.TulipFontAwesome.Yahoo` (:faicon:`yahoo`)
  * :const:`tlp.TulipFontAwesome.Yc` (:faicon:`yc`)
  * :const:`tlp.TulipFontAwesome.YcSquare` (:faicon:`yc-square`)
  * :const:`tlp.TulipFontAwesome.Yelp` (:faicon:`yelp`)
  * :const:`tlp.TulipFontAwesome.Yen` (:faicon:`yen`)
  * :const:`tlp.TulipFontAwesome.Youtube` (:faicon:`youtube`)
  * :const:`tlp.TulipFontAwesome.YoutubePlay` (:faicon:`youtube-play`)
  * :const:`tlp.TulipFontAwesome.YoutubeSquare` (:faicon:`youtube-square`)

Since the release of Tulip 4.8.1, the following icons are also available (based on Font Awesome 4.5):

  * :const:`tlp.TulipFontAwesome.Bluetooth` (:faicon:`bluetooth`)
  * :const:`tlp.TulipFontAwesome.BluetoothB` (:faicon:`bluetooth-b`)
  * :const:`tlp.TulipFontAwesome.Codiepie` (:faicon:`codiepie`)
  * :const:`tlp.TulipFontAwesome.CreditCardAlt` (:faicon:`credit-card-alt`)
  * :const:`tlp.TulipFontAwesome.Edge` (:faicon:`edge`)
  * :const:`tlp.TulipFontAwesome.FortAwesome` (:faicon:`fort-awesome`)
  * :const:`tlp.TulipFontAwesome.Hashtag` (:faicon:`hashtag`)
  * :const:`tlp.TulipFontAwesome.Mixcloud` (:faicon:`mixcloud`)
  * :const:`tlp.TulipFontAwesome.Modx` (:faicon:`modx`)
  * :const:`tlp.TulipFontAwesome.PauseCircle` (:faicon:`pause-circle`)
  * :const:`tlp.TulipFontAwesome.PauseCircleO` (:faicon:`pause-circle-o`)
  * :const:`tlp.TulipFontAwesome.Percent` (:faicon:`percent`)
  * :const:`tlp.TulipFontAwesome.ProductHunt` (:faicon:`product-hunt`)
  * :const:`tlp.TulipFontAwesome.RedditAlien` (:faicon:`reddit-alien`)
  * :const:`tlp.TulipFontAwesome.Scribd` (:faicon:`scribd`)
  * :const:`tlp.TulipFontAwesome.ShoppingBag` (:faicon:`shopping-bag`)
  * :const:`tlp.TulipFontAwesome.ShoppingBasket` (:faicon:`shopping-basket`)
  * :const:`tlp.TulipFontAwesome.StopCircle` (:faicon:`stop-circle`)
  * :const:`tlp.TulipFontAwesome.StopCircleO` (:faicon:`stop-circle-o`)
  * :const:`tlp.TulipFontAwesome.Usb` (:faicon:`usb`)

Since the release of Tulip 4.10, the following icons are also available (based on Font Awesome 4.6):

  * :const:`tlp.TulipFontAwesome.AmericanSignLanguageInterpreting` (:faicon:`american-sign-language-interpreting`)
  * :const:`tlp.TulipFontAwesome.AslInterpreting` (:faicon:`asl-interpreting`)
  * :const:`tlp.TulipFontAwesome.AssistiveListeningSystems` (:faicon:`assistive-listening-systems`)
  * :const:`tlp.TulipFontAwesome.AudioDescription` (:faicon:`audio-description`)
  * :const:`tlp.TulipFontAwesome.Blind` (:faicon:`blind`)
  * :const:`tlp.TulipFontAwesome.Braille` (:faicon:`braille`)
  * :const:`tlp.TulipFontAwesome.Deaf` (:faicon:`deaf`)
  * :const:`tlp.TulipFontAwesome.Deafness` (:faicon:`deafness`)
  * :const:`tlp.TulipFontAwesome.Envira` (:faicon:`envira`)
  * :const:`tlp.TulipFontAwesome.Fa` (:faicon:`fa`)
  * :const:`tlp.TulipFontAwesome.FirstOrder` (:faicon:`first-order`)
  * :const:`tlp.TulipFontAwesome.FontAwesome` (:faicon:`font-awesome`)
  * :const:`tlp.TulipFontAwesome.Gitlab` (:faicon:`gitlab`)
  * :const:`tlp.TulipFontAwesome.Glide` (:faicon:`glide`)
  * :const:`tlp.TulipFontAwesome.GlideG` (:faicon:`glide-g`)
  * :const:`tlp.TulipFontAwesome.GooglePlusCircle` (:faicon:`google-plus-circle`)
  * :const:`tlp.TulipFontAwesome.GooglePlusOfficial` (:faicon:`google-plus-official`)
  * :const:`tlp.TulipFontAwesome.HardOfHearing` (:faicon:`hard-of-hearing`)
  * :const:`tlp.TulipFontAwesome.LowVision` (:faicon:`low-vision`)
  * :const:`tlp.TulipFontAwesome.QuestionCircleO` (:faicon:`question-circle-o`)
  * :const:`tlp.TulipFontAwesome.SignLanguage` (:faicon:`sign-language`)
  * :const:`tlp.TulipFontAwesome.Signing` (:faicon:`signing`)
  * :const:`tlp.TulipFontAwesome.Snapchat` (:faicon:`snapchat`)
  * :const:`tlp.TulipFontAwesome.SnapchatGhost` (:faicon:`snapchat-ghost`)
  * :const:`tlp.TulipFontAwesome.SnapchatSquare` (:faicon:`snapchat-square`)
  * :const:`tlp.TulipFontAwesome.Themeisle` (:faicon:`themeisle`)
  * :const:`tlp.TulipFontAwesome.UniversalAccess` (:faicon:`universal-access`)
  * :const:`tlp.TulipFontAwesome.Viadeo` (:faicon:`viadeo`)
  * :const:`tlp.TulipFontAwesome.ViadeoSquare` (:faicon:`viadeo-square`)
  * :const:`tlp.TulipFontAwesome.VolumeControlPhone` (:faicon:`volume-control-phone`)
  * :const:`tlp.TulipFontAwesome.WheelchairAlt` (:faicon:`wheelchair-alt`)
  * :const:`tlp.TulipFontAwesome.Wpbeginner` (:faicon:`wpbeginner`)
  * :const:`tlp.TulipFontAwesome.Wpforms` (:faicon:`wpforms`)
  * :const:`tlp.TulipFontAwesome.Yoast` (:faicon:`yoast`)


%End

public:

  static std::string getFontAwesomeTrueTypeFileLocation();
%Docstring
tlp.TulipFontAwesome.getFontAwesomeTrueTypeFileLocation()

Returns the location of the Font Awesome TrueType font file bundled with Tulip.

:rtype: string
%End

  static const std::vector<std::string> &getSupportedFontAwesomeIcons();
%Docstring
tlp.TulipFontAwesome.getSupportedFontAwesomeIcons()

Returns the list of supported Font Awesome icons names.

:rtype: list of string
%End

  static bool isFontAwesomeIconSupported(const std::string &iconName);
%Docstring
tlp.TulipFontAwesome.isFontAwesomeIconSupported(iconName)

Checks if the provided Font Awesome icon name is supported.

:param iconName: the name of the icon to check support
:type iconName: string
:rtype: boolean
%End

  static unsigned int getFontAwesomeIconCodePoint(const std::string &iconName);
%Docstring
tlp.TulipFontAwesome.getFontAwesomeIconCodePoint(iconName)

Returns the Unicode code point associated to an icon name.

:param iconName: the name of the icon to get the codepoint
:type iconName: string
:rtype: integer
%End


  static std::string getFontAwesomeIconUtf8String(const std::string &iconName);
%Docstring
tlp.TulipFontAwesome.getFontAwesomeIconUtf8String(iconName)

Returns an UTF-8 encoded string of a Font Awesome icon.

:param iconName: a Font Awesome icon name
:type iconName: string
:rtype: string
%End

  static const char* Adjust;
  static const char* Adn;
  static const char* AlignCenter;
  static const char* AlignJustify;
  static const char* AlignLeft;
  static const char* AlignRight;
  static const char* Ambulance;
  static const char* Anchor;
  static const char* Android;
  static const char* Angellist;
  static const char* AngleDoubleDown;
  static const char* AngleDoubleLeft;
  static const char* AngleDoubleRight;
  static const char* AngleDoubleUp;
  static const char* AngleDown;
  static const char* AngleLeft;
  static const char* AngleRight;
  static const char* AngleUp;
  static const char* Apple;
  static const char* Archive;
  static const char* AreaChart;
  static const char* ArrowCircleDown;
  static const char* ArrowCircleLeft;
  static const char* ArrowCircleODown;
  static const char* ArrowCircleOLeft;
  static const char* ArrowCircleORight;
  static const char* ArrowCircleOUp;
  static const char* ArrowCircleRight;
  static const char* ArrowCircleUp;
  static const char* ArrowDown;
  static const char* ArrowLeft;
  static const char* ArrowRight;
  static const char* ArrowUp;
  static const char* Arrows;
  static const char* ArrowsAlt;
  static const char* ArrowsH;
  static const char* ArrowsV;
  static const char* Asterisk;
  static const char* At;
  static const char* Automobile;
  static const char* Backward;
  static const char* Ban;
  static const char* Bank;
  static const char* BarChart;
  static const char* BarChartO;
  static const char* Barcode;
  static const char* Bars;
  static const char* Bed;
  static const char* Beer;
  static const char* Behance;
  static const char* BehanceSquare;
  static const char* Bell;
  static const char* BellO;
  static const char* BellSlash;
  static const char* BellSlashO;
  static const char* Bicycle;
  static const char* Binoculars;
  static const char* BirthdayCake;
  static const char* Bitbucket;
  static const char* BitbucketSquare;
  static const char* Bitcoin;
  static const char* Bold;
  static const char* Bolt;
  static const char* Bomb;
  static const char* Book;
  static const char* Bookmark;
  static const char* BookmarkO;
  static const char* Briefcase;
  static const char* Btc;
  static const char* Bug;
  static const char* Building;
  static const char* BuildingO;
  static const char* Bullhorn;
  static const char* Bullseye;
  static const char* Bus;
  static const char* Buysellads;
  static const char* Cab;
  static const char* Calculator;
  static const char* Calendar;
  static const char* CalendarO;
  static const char* Camera;
  static const char* CameraRetro;
  static const char* Car;
  static const char* CaretDown;
  static const char* CaretLeft;
  static const char* CaretRight;
  static const char* CaretSquareODown;
  static const char* CaretSquareOLeft;
  static const char* CaretSquareORight;
  static const char* CaretSquareOUp;
  static const char* CaretUp;
  static const char* CartArrowDown;
  static const char* CartPlus;
  static const char* Cc;
  static const char* CcAmex;
  static const char* CcDiscover;
  static const char* CcMastercard;
  static const char* CcPaypal;
  static const char* CcStripe;
  static const char* CcVisa;
  static const char* Certificate;
  static const char* Chain;
  static const char* ChainBroken;
  static const char* Check;
  static const char* CheckCircle;
  static const char* CheckCircleO;
  static const char* CheckSquare;
  static const char* CheckSquareO;
  static const char* ChevronCircleDown;
  static const char* ChevronCircleLeft;
  static const char* ChevronCircleRight;
  static const char* ChevronCircleUp;
  static const char* ChevronDown;
  static const char* ChevronLeft;
  static const char* ChevronRight;
  static const char* ChevronUp;
  static const char* Child;
  static const char* Circle;
  static const char* CircleO;
  static const char* CircleONotch;
  static const char* CircleThin;
  static const char* Clipboard;
  static const char* ClockO;
  static const char* Close;
  static const char* Cloud;
  static const char* CloudDownload;
  static const char* CloudUpload;
  static const char* Cny;
  static const char* Code;
  static const char* CodeFork;
  static const char* Codepen;
  static const char* Coffee;
  static const char* Cog;
  static const char* Cogs;
  static const char* Columns;
  static const char* Comment;
  static const char* CommentO;
  static const char* Comments;
  static const char* CommentsO;
  static const char* Compass;
  static const char* Compress;
  static const char* Connectdevelop;
  static const char* Copy;
  static const char* Copyright;
  static const char* CreditCard;
  static const char* Crop;
  static const char* Crosshairs;
  static const char* Css3;
  static const char* Cube;
  static const char* Cubes;
  static const char* Cut;
  static const char* Cutlery;
  static const char* Dashboard;
  static const char* Dashcube;
  static const char* Database;
  static const char* Dedent;
  static const char* Delicious;
  static const char* Desktop;
  static const char* Deviantart;
  static const char* Diamond;
  static const char* Digg;
  static const char* Dollar;
  static const char* DotCircleO;
  static const char* Download;
  static const char* Dribbble;
  static const char* Dropbox;
  static const char* Drupal;
  static const char* Edit;
  static const char* Eject;
  static const char* EllipsisH;
  static const char* EllipsisV;
  static const char* Empire;
  static const char* Envelope;
  static const char* EnvelopeO;
  static const char* EnvelopeSquare;
  static const char* Eraser;
  static const char* Eur;
  static const char* Euro;
  static const char* Exchange;
  static const char* Exclamation;
  static const char* ExclamationCircle;
  static const char* ExclamationTriangle;
  static const char* Expand;
  static const char* ExternalLink;
  static const char* ExternalLinkSquare;
  static const char* Eye;
  static const char* EyeSlash;
  static const char* Eyedropper;
  static const char* Facebook;
  static const char* FacebookF;
  static const char* FacebookOfficial;
  static const char* FacebookSquare;
  static const char* FastBackward;
  static const char* FastForward;
  static const char* Fax;
  static const char* Female;
  static const char* FighterJet;
  static const char* File;
  static const char* FileArchiveO;
  static const char* FileAudioO;
  static const char* FileCodeO;
  static const char* FileExcelO;
  static const char* FileImageO;
  static const char* FileMovieO;
  static const char* FileO;
  static const char* FilePdfO;
  static const char* FilePhotoO;
  static const char* FilePictureO;
  static const char* FilePowerpointO;
  static const char* FileSoundO;
  static const char* FileText;
  static const char* FileTextO;
  static const char* FileVideoO;
  static const char* FileWordO;
  static const char* FileZipO;
  static const char* FilesO;
  static const char* Film;
  static const char* Filter;
  //static const char* Fire;
  static const char* FireExtinguisher;
  static const char* Flag;
  static const char* FlagCheckered;
  static const char* FlagO;
  static const char* Flash;
  static const char* Flask;
  static const char* Flickr;
  static const char* FloppyO;
  static const char* Folder;
  static const char* FolderO;
  static const char* FolderOpen;
  static const char* FolderOpenO;
  static const char* Font;
  static const char* Forumbee;
  static const char* Forward;
  static const char* Foursquare;
  static const char* FrownO;
  static const char* FutbolO;
  static const char* Gamepad;
  static const char* Gavel;
  static const char* Gbp;
  static const char* Ge;
  static const char* Gear;
  static const char* Gears;
  static const char* Genderless;
  static const char* Gift;
  static const char* Git;
  static const char* GitSquare;
  static const char* Github;
  static const char* GithubAlt;
  static const char* GithubSquare;
  static const char* Gittip;
  static const char* Glass;
  static const char* Globe;
  static const char* Google;
  static const char* GooglePlus;
  static const char* GooglePlusSquare;
  static const char* GoogleWallet;
  static const char* GraduationCap;
  static const char* Gratipay;
  static const char* Group;
  static const char* HSquare;
  static const char* HackerNews;
  static const char* HandODown;
  static const char* HandOLeft;
  static const char* HandORight;
  static const char* HandOUp;
  static const char* HddO;
  static const char* Header;
  static const char* Headphones;
  static const char* Heart;
  static const char* HeartO;
  static const char* Heartbeat;
  static const char* History;
  static const char* Home;
  static const char* HospitalO;
  static const char* Hotel;
  static const char* Html5;
  static const char* Ils;
  static const char* Image;
  static const char* Inbox;
  static const char* Indent;
  static const char* Info;
  static const char* InfoCircle;
  static const char* Inr;
  static const char* Instagram;
  static const char* Institution;
  static const char* Ioxhost;
  static const char* Italic;
  static const char* Joomla;
  static const char* Jpy;
  static const char* Jsfiddle;
  static const char* Key;
  static const char* KeyboardO;
  static const char* Krw;
  static const char* Language;
  static const char* Laptop;
  static const char* Lastfm;
  static const char* LastfmSquare;
  static const char* Leaf;
  static const char* Leanpub;
  static const char* Legal;
  static const char* LemonO;
  static const char* LevelDown;
  static const char* LevelUp;
  static const char* LifeBouy;
  static const char* LifeBuoy;
  static const char* LifeRing;
  static const char* LifeSaver;
  static const char* LightbulbO;
  static const char* LineChart;
  static const char* Link;
  static const char* Linkedin;
  static const char* LinkedinSquare;
  static const char* Linux;
  static const char* List;
  static const char* ListAlt;
  static const char* ListOl;
  static const char* ListUl;
  static const char* LocationArrow;
  static const char* Lock;
  static const char* LongArrowDown;
  static const char* LongArrowLeft;
  static const char* LongArrowRight;
  static const char* LongArrowUp;
  static const char* Magic;
  static const char* Magnet;
  static const char* MailForward;
  static const char* MailReply;
  static const char* MailReplyAll;
  static const char* Male;
  static const char* MapMarker;
  static const char* Mars;
  static const char* MarsDouble;
  static const char* MarsStroke;
  static const char* MarsStrokeH;
  static const char* MarsStrokeV;
  static const char* Maxcdn;
  static const char* Meanpath;
  static const char* Medium;
  static const char* Medkit;
  static const char* MehO;
  static const char* Mercury;
  static const char* Microphone;
  static const char* MicrophoneSlash;
  static const char* Minus;
  static const char* MinusCircle;
  static const char* MinusSquare;
  static const char* MinusSquareO;
  static const char* Mobile;
  static const char* MobilePhone;
  static const char* Money;
  static const char* MoonO;
  static const char* MortarBoard;
  static const char* Motorcycle;
  static const char* Music;
  static const char* Navicon;
  static const char* Neuter;
  static const char* NewspaperO;
  static const char* Openid;
  static const char* Outdent;
  static const char* Pagelines;
  static const char* PaintBrush;
  static const char* PaperPlane;
  static const char* PaperPlaneO;
  static const char* Paperclip;
  static const char* Paragraph;
  static const char* Paste;
  static const char* Pause;
  static const char* Paw;
  static const char* Paypal;
  static const char* Pencil;
  static const char* PencilSquare;
  static const char* PencilSquareO;
  static const char* Phone;
  static const char* PhoneSquare;
  static const char* Photo;
  static const char* PictureO;
  static const char* PieChart;
  static const char* PiedPiper;
  static const char* PiedPiperAlt;
  static const char* Pinterest;
  static const char* PinterestP;
  static const char* PinterestSquare;
  static const char* Plane;
  static const char* Play;
  static const char* PlayCircle;
  static const char* PlayCircleO;
  static const char* Plug;
  static const char* Plus;
  static const char* PlusCircle;
  static const char* PlusSquare;
  static const char* PlusSquareO;
  static const char* PowerOff;
  static const char* Print;
  static const char* PuzzlePiece;
  static const char* Qq;
  static const char* Qrcode;
  static const char* Question;
  static const char* QuestionCircle;
  static const char* QuoteLeft;
  static const char* QuoteRight;
  static const char* Ra;
  static const char* Random;
  static const char* Rebel;
  static const char* Recycle;
  static const char* Reddit;
  static const char* RedditSquare;
  static const char* Refresh;
  static const char* Remove;
  static const char* Renren;
  static const char* Reorder;
  static const char* Repeat;
  static const char* Reply;
  static const char* ReplyAll;
  static const char* Retweet;
  static const char* Rmb;
  static const char* Road;
  static const char* Rocket;
  static const char* RotateLeft;
  static const char* RotateRight;
  static const char* Rouble;
  static const char* Rss;
  static const char* RssSquare;
  static const char* Rub;
  static const char* Ruble;
  static const char* Rupee;
  static const char* Save;
  static const char* Scissors;
  static const char* Search;
  static const char* SearchMinus;
  static const char* SearchPlus;
  static const char* Sellsy;
  static const char* Send;
  static const char* SendO;
  static const char* Server;
  static const char* Share;
  static const char* ShareAlt;
  static const char* ShareAltSquare;
  static const char* ShareSquare;
  static const char* ShareSquareO;
  static const char* Shekel;
  static const char* Sheqel;
  static const char* Shield;
  static const char* Ship;
  static const char* Shirtsinbulk;
  static const char* ShoppingCart;
  static const char* SignIn;
  static const char* SignOut;
  static const char* Signal;
  static const char* Simplybuilt;
  static const char* Sitemap;
  static const char* Skyatlas;
  static const char* Skype;
  static const char* Slack;
  static const char* Sliders;
  static const char* Slideshare;
  static const char* SmileO;
  static const char* SoccerBallO;
  static const char* Sort;
  static const char* SortAlphaAsc;
  static const char* SortAlphaDesc;
  static const char* SortAmountAsc;
  static const char* SortAmountDesc;
  static const char* SortAsc;
  static const char* SortDesc;
  static const char* SortDown;
  static const char* SortNumericAsc;
  static const char* SortNumericDesc;
  static const char* SortUp;
  static const char* Soundcloud;
  static const char* SpaceShuttle;
  static const char* Spinner;
  static const char* Spoon;
  static const char* Spotify;
  static const char* Square;
  static const char* SquareO;
  static const char* StackExchange;
  static const char* StackOverflow;
  static const char* Star;
  static const char* StarHalf;
  static const char* StarHalfEmpty;
  static const char* StarHalfFull;
  static const char* StarHalfO;
  static const char* StarO;
  static const char* Steam;
  static const char* SteamSquare;
  static const char* StepBackward;
  static const char* StepForward;
  static const char* Stethoscope;
  static const char* Stop;
  static const char* StreetView;
  static const char* Strikethrough;
  static const char* Stumbleupon;
  static const char* StumbleuponCircle;
  static const char* Subscript;
  static const char* Subway;
  static const char* Suitcase;
  static const char* SunO;
  static const char* Superscript;
  static const char* Support;
  static const char* Table;
  static const char* Tablet;
  static const char* Tachometer;
  static const char* Tag;
  static const char* Tags;
  static const char* Tasks;
  static const char* Taxi;
  static const char* TencentWeibo;
  static const char* Terminal;
  static const char* TextHeight;
  static const char* TextWidth;
  static const char* Th;
  static const char* ThLarge;
  static const char* ThList;
  static const char* ThumbTack;
  static const char* ThumbsDown;
  static const char* ThumbsODown;
  static const char* ThumbsOUp;
  static const char* ThumbsUp;
  static const char* Ticket;
  static const char* Times;
  static const char* TimesCircle;
  static const char* TimesCircleO;
  static const char* Tint;
  static const char* ToggleDown;
  static const char* ToggleLeft;
  static const char* ToggleOff;
  static const char* ToggleOn;
  static const char* ToggleRight;
  static const char* ToggleUp;
  static const char* Train;
  static const char* Transgender;
  static const char* TransgenderAlt;
  static const char* Trash;
  static const char* TrashO;
  static const char* Tree;
  static const char* Trello;
  static const char* Trophy;
  static const char* Truck;
  static const char* Try;
  static const char* Tty;
  static const char* Tumblr;
  static const char* TumblrSquare;
  static const char* TurkishLira;
  static const char* Twitch;
  static const char* Twitter;
  static const char* TwitterSquare;
  static const char* Umbrella;
  static const char* Underline;
  static const char* Undo;
  static const char* University;
  static const char* Unlink;
  static const char* Unlock;
  static const char* UnlockAlt;
  static const char* Unsorted;
  static const char* Upload;
  static const char* Usd;
  static const char* User;
  static const char* UserMd;
  static const char* UserPlus;
  static const char* UserSecret;
  static const char* UserTimes;
  static const char* Users;
  static const char* Venus;
  static const char* VenusDouble;
  static const char* VenusMars;
  static const char* Viacoin;
  static const char* VideoCamera;
  static const char* VimeoSquare;
  static const char* Vine;
  static const char* Vk;
  static const char* VolumeDown;
  static const char* VolumeOff;
  static const char* VolumeUp;
  static const char* Warning;
  static const char* Wechat;
  static const char* Weibo;
  static const char* Weixin;
  static const char* Whatsapp;
  static const char* Wheelchair;
  static const char* Wifi;
  static const char* Windows;
  static const char* Won;
  static const char* Wordpress;
  static const char* Wrench;
  static const char* Xing;
  static const char* XingSquare;
  static const char* Yahoo;
  static const char* Yelp;
  static const char* Yen;
  static const char* Youtube;
  static const char* YoutubePlay;
  static const char* YoutubeSquare;

  static const char* Yc;
  static const char* YCombinator;
  //static const char* OptinMonster;
  static const char* Opencart;
  static const char* Expeditedssl;
  static const char* Battery4;
  static const char* BatteryFull;
  static const char* Battery3;
  static const char* BatteryThreeQuarters;
  static const char* Battery2;
  static const char* BatteryHalf;
  static const char* Battery1;
  static const char* BatteryQuarter;
  static const char* Battery0;
  static const char* BatteryEmpty;
  static const char* MousePointer;
  static const char* ICursor;
  static const char* ObjectGroup;
  static const char* ObjectUngroup;
  static const char* StickyNote;
  static const char* StickyNoteO;
  static const char* CcJcb;
  static const char* CcDinersClub;
  static const char* Clone;
  static const char* BalanceScale;
  static const char* HourglassO;
  static const char* Hourglass1;
  static const char* HourglassStart;
  static const char* Hourglass2;
  static const char* HourglassHalf;
  static const char* Hourglass3;
  static const char* HourglassEnd;
  static const char* Hourglass;
  static const char* HandGrabO;
  static const char* HandRockO;
  static const char* HandStopO;
  static const char* HandPaperO;
  static const char* HandScissorsO;
  static const char* HandLizardO;
  static const char* HandSpockO;
  static const char* HandPointerO;
  static const char* HandPeaceO;
  static const char* Trademark;
  static const char* Registered;
  static const char* CreativeCommons;
  static const char* Gg;
  static const char* GgCircle;
  static const char* Tripadvisor;
  static const char* Odnoklassniki;
  static const char* OdnoklassnikiSquare;
  static const char* GetPocket;
  static const char* WikipediaW;
  static const char* Safari;
  static const char* Chrome;
  static const char* Firefox;
  static const char* Opera;
  static const char* InternetExplorer;
  static const char* Tv;
  static const char* Television;
  static const char* Contao;
  static const char* Px500;
  static const char* Amazon;
  static const char* CalendarPlusO;
  static const char* CalendarMinusO;
  static const char* CalendarTimesO;
  static const char* CalendarCheckO;
  static const char* Industry;
  static const char* MapPin;
  static const char* MapSigns;
  static const char* MapO;
  static const char* Map;
  static const char* Commenting;
  static const char* CommentingO;
  static const char* Houzz;
  static const char* Vimeo;
  static const char* BlackTie;
  static const char* Fonticons;

  static const char* AmericanSignLanguageInterpreting;
  static const char* AslInterpreting;
  static const char* AssistiveListeningSystems;
  static const char* AudioDescription;
  static const char* Blind;
  static const char* Braille;
  static const char* Deaf;
  static const char* Deafness;
  static const char* Envira;
  static const char* Fa;
  static const char* FirstOrder;
  static const char* FontAwesome;
  static const char* Gitlab;
  static const char* Glide;
  static const char* GlideG;
  static const char* GooglePlusCircle;
  static const char* GooglePlusOfficial;
  static const char* HardOfHearing;
  static const char* LowVision;
  static const char* QuestionCircleO;
  static const char* SignLanguage;
  static const char* Signing;
  static const char* Snapchat;
  static const char* SnapchatGhost;
  static const char* SnapchatSquare;
  static const char* Themeisle;
  static const char* UniversalAccess;
  static const char* Viadeo;
  static const char* ViadeoSquare;
  static const char* VolumeControlPhone;
  static const char* WheelchairAlt;
  static const char* Wpbeginner;
  static const char* Wpforms;
  static const char* Yoast;


};

};
