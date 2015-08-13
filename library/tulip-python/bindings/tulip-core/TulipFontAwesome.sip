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

The following string constants representing the supported icons are available :

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
      * :const:`tlp.TulipFontAwesome.Fire` (:faicon:`fire`)
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
      * :const:`tlp.TulipFontAwesome.OptinMonster` (:faicon:`optin-monster`)
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

%End

public:

  static std::string getFontAwesomeTrueTypeFileLocation();
%Docstring
tlp.TulipFontAwesome.getFontAwesomeTrueTypeFileLocation()

Returns the location of the Font Awesome ttf file bundled with Tulip.

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

  static const std::string Adjust;
  static const std::string Adn;
  static const std::string AlignCenter;
  static const std::string AlignJustify;
  static const std::string AlignLeft;
  static const std::string AlignRight;
  static const std::string Ambulance;
  static const std::string Anchor;
  static const std::string Android;
  static const std::string Angellist;
  static const std::string AngleDoubleDown;
  static const std::string AngleDoubleLeft;
  static const std::string AngleDoubleRight;
  static const std::string AngleDoubleUp;
  static const std::string AngleDown;
  static const std::string AngleLeft;
  static const std::string AngleRight;
  static const std::string AngleUp;
  static const std::string Apple;
  static const std::string Archive;
  static const std::string AreaChart;
  static const std::string ArrowCircleDown;
  static const std::string ArrowCircleLeft;
  static const std::string ArrowCircleODown;
  static const std::string ArrowCircleOLeft;
  static const std::string ArrowCircleORight;
  static const std::string ArrowCircleOUp;
  static const std::string ArrowCircleRight;
  static const std::string ArrowCircleUp;
  static const std::string ArrowDown;
  static const std::string ArrowLeft;
  static const std::string ArrowRight;
  static const std::string ArrowUp;
  static const std::string Arrows;
  static const std::string ArrowsAlt;
  static const std::string ArrowsH;
  static const std::string ArrowsV;
  static const std::string Asterisk;
  static const std::string At;
  static const std::string Automobile;
  static const std::string Backward;
  static const std::string Ban;
  static const std::string Bank;
  static const std::string BarChart;
  static const std::string BarChartO;
  static const std::string Barcode;
  static const std::string Bars;
  static const std::string Bed;
  static const std::string Beer;
  static const std::string Behance;
  static const std::string BehanceSquare;
  static const std::string Bell;
  static const std::string BellO;
  static const std::string BellSlash;
  static const std::string BellSlashO;
  static const std::string Bicycle;
  static const std::string Binoculars;
  static const std::string BirthdayCake;
  static const std::string Bitbucket;
  static const std::string BitbucketSquare;
  static const std::string Bitcoin;
  static const std::string Bold;
  static const std::string Bolt;
  static const std::string Bomb;
  static const std::string Book;
  static const std::string Bookmark;
  static const std::string BookmarkO;
  static const std::string Briefcase;
  static const std::string Btc;
  static const std::string Bug;
  static const std::string Building;
  static const std::string BuildingO;
  static const std::string Bullhorn;
  static const std::string Bullseye;
  static const std::string Bus;
  static const std::string Buysellads;
  static const std::string Cab;
  static const std::string Calculator;
  static const std::string Calendar;
  static const std::string CalendarO;
  static const std::string Camera;
  static const std::string CameraRetro;
  static const std::string Car;
  static const std::string CaretDown;
  static const std::string CaretLeft;
  static const std::string CaretRight;
  static const std::string CaretSquareODown;
  static const std::string CaretSquareOLeft;
  static const std::string CaretSquareORight;
  static const std::string CaretSquareOUp;
  static const std::string CaretUp;
  static const std::string CartArrowDown;
  static const std::string CartPlus;
  static const std::string Cc;
  static const std::string CcAmex;
  static const std::string CcDiscover;
  static const std::string CcMastercard;
  static const std::string CcPaypal;
  static const std::string CcStripe;
  static const std::string CcVisa;
  static const std::string Certificate;
  static const std::string Chain;
  static const std::string ChainBroken;
  static const std::string Check;
  static const std::string CheckCircle;
  static const std::string CheckCircleO;
  static const std::string CheckSquare;
  static const std::string CheckSquareO;
  static const std::string ChevronCircleDown;
  static const std::string ChevronCircleLeft;
  static const std::string ChevronCircleRight;
  static const std::string ChevronCircleUp;
  static const std::string ChevronDown;
  static const std::string ChevronLeft;
  static const std::string ChevronRight;
  static const std::string ChevronUp;
  static const std::string Child;
  static const std::string Circle;
  static const std::string CircleO;
  static const std::string CircleONotch;
  static const std::string CircleThin;
  static const std::string Clipboard;
  static const std::string ClockO;
  static const std::string Close;
  static const std::string Cloud;
  static const std::string CloudDownload;
  static const std::string CloudUpload;
  static const std::string Cny;
  static const std::string Code;
  static const std::string CodeFork;
  static const std::string Codepen;
  static const std::string Coffee;
  static const std::string Cog;
  static const std::string Cogs;
  static const std::string Columns;
  static const std::string Comment;
  static const std::string CommentO;
  static const std::string Comments;
  static const std::string CommentsO;
  static const std::string Compass;
  static const std::string Compress;
  static const std::string Connectdevelop;
  static const std::string Copy;
  static const std::string Copyright;
  static const std::string CreditCard;
  static const std::string Crop;
  static const std::string Crosshairs;
  static const std::string Css3;
  static const std::string Cube;
  static const std::string Cubes;
  static const std::string Cut;
  static const std::string Cutlery;
  static const std::string Dashboard;
  static const std::string Dashcube;
  static const std::string Database;
  static const std::string Dedent;
  static const std::string Delicious;
  static const std::string Desktop;
  static const std::string Deviantart;
  static const std::string Diamond;
  static const std::string Digg;
  static const std::string Dollar;
  static const std::string DotCircleO;
  static const std::string Download;
  static const std::string Dribbble;
  static const std::string Dropbox;
  static const std::string Drupal;
  static const std::string Edit;
  static const std::string Eject;
  static const std::string EllipsisH;
  static const std::string EllipsisV;
  static const std::string Empire;
  static const std::string Envelope;
  static const std::string EnvelopeO;
  static const std::string EnvelopeSquare;
  static const std::string Eraser;
  static const std::string Eur;
  static const std::string Euro;
  static const std::string Exchange;
  static const std::string Exclamation;
  static const std::string ExclamationCircle;
  static const std::string ExclamationTriangle;
  static const std::string Expand;
  static const std::string ExternalLink;
  static const std::string ExternalLinkSquare;
  static const std::string Eye;
  static const std::string EyeSlash;
  static const std::string Eyedropper;
  static const std::string Facebook;
  static const std::string FacebookF;
  static const std::string FacebookOfficial;
  static const std::string FacebookSquare;
  static const std::string FastBackward;
  static const std::string FastForward;
  static const std::string Fax;
  static const std::string Female;
  static const std::string FighterJet;
  static const std::string File;
  static const std::string FileArchiveO;
  static const std::string FileAudioO;
  static const std::string FileCodeO;
  static const std::string FileExcelO;
  static const std::string FileImageO;
  static const std::string FileMovieO;
  static const std::string FileO;
  static const std::string FilePdfO;
  static const std::string FilePhotoO;
  static const std::string FilePictureO;
  static const std::string FilePowerpointO;
  static const std::string FileSoundO;
  static const std::string FileText;
  static const std::string FileTextO;
  static const std::string FileVideoO;
  static const std::string FileWordO;
  static const std::string FileZipO;
  static const std::string FilesO;
  static const std::string Film;
  static const std::string Filter;
  static const std::string Fire;
  static const std::string FireExtinguisher;
  static const std::string Flag;
  static const std::string FlagCheckered;
  static const std::string FlagO;
  static const std::string Flash;
  static const std::string Flask;
  static const std::string Flickr;
  static const std::string FloppyO;
  static const std::string Folder;
  static const std::string FolderO;
  static const std::string FolderOpen;
  static const std::string FolderOpenO;
  static const std::string Font;
  static const std::string Forumbee;
  static const std::string Forward;
  static const std::string Foursquare;
  static const std::string FrownO;
  static const std::string FutbolO;
  static const std::string Gamepad;
  static const std::string Gavel;
  static const std::string Gbp;
  static const std::string Ge;
  static const std::string Gear;
  static const std::string Gears;
  static const std::string Genderless;
  static const std::string Gift;
  static const std::string Git;
  static const std::string GitSquare;
  static const std::string Github;
  static const std::string GithubAlt;
  static const std::string GithubSquare;
  static const std::string Gittip;
  static const std::string Glass;
  static const std::string Globe;
  static const std::string Google;
  static const std::string GooglePlus;
  static const std::string GooglePlusSquare;
  static const std::string GoogleWallet;
  static const std::string GraduationCap;
  static const std::string Gratipay;
  static const std::string Group;
  static const std::string HSquare;
  static const std::string HackerNews;
  static const std::string HandODown;
  static const std::string HandOLeft;
  static const std::string HandORight;
  static const std::string HandOUp;
  static const std::string HddO;
  static const std::string Header;
  static const std::string Headphones;
  static const std::string Heart;
  static const std::string HeartO;
  static const std::string Heartbeat;
  static const std::string History;
  static const std::string Home;
  static const std::string HospitalO;
  static const std::string Hotel;
  static const std::string Html5;
  static const std::string Ils;
  static const std::string Image;
  static const std::string Inbox;
  static const std::string Indent;
  static const std::string Info;
  static const std::string InfoCircle;
  static const std::string Inr;
  static const std::string Instagram;
  static const std::string Institution;
  static const std::string Ioxhost;
  static const std::string Italic;
  static const std::string Joomla;
  static const std::string Jpy;
  static const std::string Jsfiddle;
  static const std::string Key;
  static const std::string KeyboardO;
  static const std::string Krw;
  static const std::string Language;
  static const std::string Laptop;
  static const std::string Lastfm;
  static const std::string LastfmSquare;
  static const std::string Leaf;
  static const std::string Leanpub;
  static const std::string Legal;
  static const std::string LemonO;
  static const std::string LevelDown;
  static const std::string LevelUp;
  static const std::string LifeBouy;
  static const std::string LifeBuoy;
  static const std::string LifeRing;
  static const std::string LifeSaver;
  static const std::string LightbulbO;
  static const std::string LineChart;
  static const std::string Link;
  static const std::string Linkedin;
  static const std::string LinkedinSquare;
  static const std::string Linux;
  static const std::string List;
  static const std::string ListAlt;
  static const std::string ListOl;
  static const std::string ListUl;
  static const std::string LocationArrow;
  static const std::string Lock;
  static const std::string LongArrowDown;
  static const std::string LongArrowLeft;
  static const std::string LongArrowRight;
  static const std::string LongArrowUp;
  static const std::string Magic;
  static const std::string Magnet;
  static const std::string MailForward;
  static const std::string MailReply;
  static const std::string MailReplyAll;
  static const std::string Male;
  static const std::string MapMarker;
  static const std::string Mars;
  static const std::string MarsDouble;
  static const std::string MarsStroke;
  static const std::string MarsStrokeH;
  static const std::string MarsStrokeV;
  static const std::string Maxcdn;
  static const std::string Meanpath;
  static const std::string Medium;
  static const std::string Medkit;
  static const std::string MehO;
  static const std::string Mercury;
  static const std::string Microphone;
  static const std::string MicrophoneSlash;
  static const std::string Minus;
  static const std::string MinusCircle;
  static const std::string MinusSquare;
  static const std::string MinusSquareO;
  static const std::string Mobile;
  static const std::string MobilePhone;
  static const std::string Money;
  static const std::string MoonO;
  static const std::string MortarBoard;
  static const std::string Motorcycle;
  static const std::string Music;
  static const std::string Navicon;
  static const std::string Neuter;
  static const std::string NewspaperO;
  static const std::string Openid;
  static const std::string Outdent;
  static const std::string Pagelines;
  static const std::string PaintBrush;
  static const std::string PaperPlane;
  static const std::string PaperPlaneO;
  static const std::string Paperclip;
  static const std::string Paragraph;
  static const std::string Paste;
  static const std::string Pause;
  static const std::string Paw;
  static const std::string Paypal;
  static const std::string Pencil;
  static const std::string PencilSquare;
  static const std::string PencilSquareO;
  static const std::string Phone;
  static const std::string PhoneSquare;
  static const std::string Photo;
  static const std::string PictureO;
  static const std::string PieChart;
  static const std::string PiedPiper;
  static const std::string PiedPiperAlt;
  static const std::string Pinterest;
  static const std::string PinterestP;
  static const std::string PinterestSquare;
  static const std::string Plane;
  static const std::string Play;
  static const std::string PlayCircle;
  static const std::string PlayCircleO;
  static const std::string Plug;
  static const std::string Plus;
  static const std::string PlusCircle;
  static const std::string PlusSquare;
  static const std::string PlusSquareO;
  static const std::string PowerOff;
  static const std::string Print;
  static const std::string PuzzlePiece;
  static const std::string Qq;
  static const std::string Qrcode;
  static const std::string Question;
  static const std::string QuestionCircle;
  static const std::string QuoteLeft;
  static const std::string QuoteRight;
  static const std::string Ra;
  static const std::string Random;
  static const std::string Rebel;
  static const std::string Recycle;
  static const std::string Reddit;
  static const std::string RedditSquare;
  static const std::string Refresh;
  static const std::string Remove;
  static const std::string Renren;
  static const std::string Reorder;
  static const std::string Repeat;
  static const std::string Reply;
  static const std::string ReplyAll;
  static const std::string Retweet;
  static const std::string Rmb;
  static const std::string Road;
  static const std::string Rocket;
  static const std::string RotateLeft;
  static const std::string RotateRight;
  static const std::string Rouble;
  static const std::string Rss;
  static const std::string RssSquare;
  static const std::string Rub;
  static const std::string Ruble;
  static const std::string Rupee;
  static const std::string Save;
  static const std::string Scissors;
  static const std::string Search;
  static const std::string SearchMinus;
  static const std::string SearchPlus;
  static const std::string Sellsy;
  static const std::string Send;
  static const std::string SendO;
  static const std::string Server;
  static const std::string Share;
  static const std::string ShareAlt;
  static const std::string ShareAltSquare;
  static const std::string ShareSquare;
  static const std::string ShareSquareO;
  static const std::string Shekel;
  static const std::string Sheqel;
  static const std::string Shield;
  static const std::string Ship;
  static const std::string Shirtsinbulk;
  static const std::string ShoppingCart;
  static const std::string SignIn;
  static const std::string SignOut;
  static const std::string Signal;
  static const std::string Simplybuilt;
  static const std::string Sitemap;
  static const std::string Skyatlas;
  static const std::string Skype;
  static const std::string Slack;
  static const std::string Sliders;
  static const std::string Slideshare;
  static const std::string SmileO;
  static const std::string SoccerBallO;
  static const std::string Sort;
  static const std::string SortAlphaAsc;
  static const std::string SortAlphaDesc;
  static const std::string SortAmountAsc;
  static const std::string SortAmountDesc;
  static const std::string SortAsc;
  static const std::string SortDesc;
  static const std::string SortDown;
  static const std::string SortNumericAsc;
  static const std::string SortNumericDesc;
  static const std::string SortUp;
  static const std::string Soundcloud;
  static const std::string SpaceShuttle;
  static const std::string Spinner;
  static const std::string Spoon;
  static const std::string Spotify;
  static const std::string Square;
  static const std::string SquareO;
  static const std::string StackExchange;
  static const std::string StackOverflow;
  static const std::string Star;
  static const std::string StarHalf;
  static const std::string StarHalfEmpty;
  static const std::string StarHalfFull;
  static const std::string StarHalfO;
  static const std::string StarO;
  static const std::string Steam;
  static const std::string SteamSquare;
  static const std::string StepBackward;
  static const std::string StepForward;
  static const std::string Stethoscope;
  static const std::string Stop;
  static const std::string StreetView;
  static const std::string Strikethrough;
  static const std::string Stumbleupon;
  static const std::string StumbleuponCircle;
  static const std::string Subscript;
  static const std::string Subway;
  static const std::string Suitcase;
  static const std::string SunO;
  static const std::string Superscript;
  static const std::string Support;
  static const std::string Table;
  static const std::string Tablet;
  static const std::string Tachometer;
  static const std::string Tag;
  static const std::string Tags;
  static const std::string Tasks;
  static const std::string Taxi;
  static const std::string TencentWeibo;
  static const std::string Terminal;
  static const std::string TextHeight;
  static const std::string TextWidth;
  static const std::string Th;
  static const std::string ThLarge;
  static const std::string ThList;
  static const std::string ThumbTack;
  static const std::string ThumbsDown;
  static const std::string ThumbsODown;
  static const std::string ThumbsOUp;
  static const std::string ThumbsUp;
  static const std::string Ticket;
  static const std::string Times;
  static const std::string TimesCircle;
  static const std::string TimesCircleO;
  static const std::string Tint;
  static const std::string ToggleDown;
  static const std::string ToggleLeft;
  static const std::string ToggleOff;
  static const std::string ToggleOn;
  static const std::string ToggleRight;
  static const std::string ToggleUp;
  static const std::string Train;
  static const std::string Transgender;
  static const std::string TransgenderAlt;
  static const std::string Trash;
  static const std::string TrashO;
  static const std::string Tree;
  static const std::string Trello;
  static const std::string Trophy;
  static const std::string Truck;
  static const std::string Try;
  static const std::string Tty;
  static const std::string Tumblr;
  static const std::string TumblrSquare;
  static const std::string TurkishLira;
  static const std::string Twitch;
  static const std::string Twitter;
  static const std::string TwitterSquare;
  static const std::string Umbrella;
  static const std::string Underline;
  static const std::string Undo;
  static const std::string University;
  static const std::string Unlink;
  static const std::string Unlock;
  static const std::string UnlockAlt;
  static const std::string Unsorted;
  static const std::string Upload;
  static const std::string Usd;
  static const std::string User;
  static const std::string UserMd;
  static const std::string UserPlus;
  static const std::string UserSecret;
  static const std::string UserTimes;
  static const std::string Users;
  static const std::string Venus;
  static const std::string VenusDouble;
  static const std::string VenusMars;
  static const std::string Viacoin;
  static const std::string VideoCamera;
  static const std::string VimeoSquare;
  static const std::string Vine;
  static const std::string Vk;
  static const std::string VolumeDown;
  static const std::string VolumeOff;
  static const std::string VolumeUp;
  static const std::string Warning;
  static const std::string Wechat;
  static const std::string Weibo;
  static const std::string Weixin;
  static const std::string Whatsapp;
  static const std::string Wheelchair;
  static const std::string Wifi;
  static const std::string Windows;
  static const std::string Won;
  static const std::string Wordpress;
  static const std::string Wrench;
  static const std::string Xing;
  static const std::string XingSquare;
  static const std::string Yahoo;
  static const std::string Yelp;
  static const std::string Yen;
  static const std::string Youtube;
  static const std::string YoutubePlay;
  static const std::string YoutubeSquare;

  static const std::string Yc;
  static const std::string YCombinator;
  static const std::string OptinMonster;
  static const std::string Opencart;
  static const std::string Expeditedssl;
  static const std::string Battery4;
  static const std::string BatteryFull;
  static const std::string Battery3;
  static const std::string BatteryThreeQuarters;
  static const std::string Battery2;
  static const std::string BatteryHalf;
  static const std::string Battery1;
  static const std::string BatteryQuarter;
  static const std::string Battery0;
  static const std::string BatteryEmpty;
  static const std::string MousePointer;
  static const std::string ICursor;
  static const std::string ObjectGroup;
  static const std::string ObjectUngroup;
  static const std::string StickyNote;
  static const std::string StickyNoteO;
  static const std::string CcJcb;
  static const std::string CcDinersClub;
  static const std::string Clone;
  static const std::string BalanceScale;
  static const std::string HourglassO;
  static const std::string Hourglass1;
  static const std::string HourglassStart;
  static const std::string Hourglass2;
  static const std::string HourglassHalf;
  static const std::string Hourglass3;
  static const std::string HourglassEnd;
  static const std::string Hourglass;
  static const std::string HandGrabO;
  static const std::string HandRockO;
  static const std::string HandStopO;
  static const std::string HandPaperO;
  static const std::string HandScissorsO;
  static const std::string HandLizardO;
  static const std::string HandSpockO;
  static const std::string HandPointerO;
  static const std::string HandPeaceO;
  static const std::string Trademark;
  static const std::string Registered;
  static const std::string CreativeCommons;
  static const std::string Gg;
  static const std::string GgCircle;
  static const std::string Tripadvisor;
  static const std::string Odnoklassniki;
  static const std::string OdnoklassnikiSquare;
  static const std::string GetPocket;
  static const std::string WikipediaW;
  static const std::string Safari;
  static const std::string Chrome;
  static const std::string Firefox;
  static const std::string Opera;
  static const std::string InternetExplorer;
  static const std::string Tv;
  static const std::string Television;
  static const std::string Contao;
  static const std::string Px500;
  static const std::string Amazon;
  static const std::string CalendarPlusO;
  static const std::string CalendarMinusO;
  static const std::string CalendarTimesO;
  static const std::string CalendarCheckO;
  static const std::string Industry;
  static const std::string MapPin;
  static const std::string MapSigns;
  static const std::string MapO;
  static const std::string Map;
  static const std::string Commenting;
  static const std::string CommentingO;
  static const std::string Houzz;
  static const std::string Vimeo;
  static const std::string BlackTie;
  static const std::string Fonticons;

};

};
