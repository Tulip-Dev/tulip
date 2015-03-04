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
and sets the "user" icon::

   # graph is a pointer to a tlp.Graph object
   viewShape = graph.getIntegerProperty("viewShape")
   viewFontAwesomeIcon = graph.getStringProperty("viewFontAwesomeIcon")

   # sets the Font Awesome glyph on all nodes
   viewShape.setAllNodeValue(tlp.NodeShape.FontAwesomeIcon)
   # sets the "user" glyph for all nodes
   viewFontAwesomeIcon.setAllNodeValue(tlp.TulipFontAwesome.User)

The following string constants representing the supported icons are available :

      * :const:`tlp.TulipFontAwesome.Adjust`
      * :const:`tlp.TulipFontAwesome.Adn`
      * :const:`tlp.TulipFontAwesome.AlignCenter`
      * :const:`tlp.TulipFontAwesome.AlignJustify`
      * :const:`tlp.TulipFontAwesome.AlignLeft`
      * :const:`tlp.TulipFontAwesome.AlignRight`
      * :const:`tlp.TulipFontAwesome.Ambulance`
      * :const:`tlp.TulipFontAwesome.Anchor`
      * :const:`tlp.TulipFontAwesome.Android`
      * :const:`tlp.TulipFontAwesome.Angellist`
      * :const:`tlp.TulipFontAwesome.AngleDoubleDown`
      * :const:`tlp.TulipFontAwesome.AngleDoubleLeft`
      * :const:`tlp.TulipFontAwesome.AngleDoubleRight`
      * :const:`tlp.TulipFontAwesome.AngleDoubleUp`
      * :const:`tlp.TulipFontAwesome.AngleDown`
      * :const:`tlp.TulipFontAwesome.AngleLeft`
      * :const:`tlp.TulipFontAwesome.AngleRight`
      * :const:`tlp.TulipFontAwesome.AngleUp`
      * :const:`tlp.TulipFontAwesome.Apple`
      * :const:`tlp.TulipFontAwesome.Archive`
      * :const:`tlp.TulipFontAwesome.AreaChart`
      * :const:`tlp.TulipFontAwesome.ArrowCircleDown`
      * :const:`tlp.TulipFontAwesome.ArrowCircleLeft`
      * :const:`tlp.TulipFontAwesome.ArrowCircleODown`
      * :const:`tlp.TulipFontAwesome.ArrowCircleOLeft`
      * :const:`tlp.TulipFontAwesome.ArrowCircleORight`
      * :const:`tlp.TulipFontAwesome.ArrowCircleOUp`
      * :const:`tlp.TulipFontAwesome.ArrowCircleRight`
      * :const:`tlp.TulipFontAwesome.ArrowCircleUp`
      * :const:`tlp.TulipFontAwesome.ArrowDown`
      * :const:`tlp.TulipFontAwesome.ArrowLeft`
      * :const:`tlp.TulipFontAwesome.ArrowRight`
      * :const:`tlp.TulipFontAwesome.ArrowUp`
      * :const:`tlp.TulipFontAwesome.Arrows`
      * :const:`tlp.TulipFontAwesome.ArrowsAlt`
      * :const:`tlp.TulipFontAwesome.ArrowsH`
      * :const:`tlp.TulipFontAwesome.ArrowsV`
      * :const:`tlp.TulipFontAwesome.Asterisk`
      * :const:`tlp.TulipFontAwesome.At`
      * :const:`tlp.TulipFontAwesome.Automobile`
      * :const:`tlp.TulipFontAwesome.Backward`
      * :const:`tlp.TulipFontAwesome.Ban`
      * :const:`tlp.TulipFontAwesome.Bank`
      * :const:`tlp.TulipFontAwesome.BarChart`
      * :const:`tlp.TulipFontAwesome.BarChartO`
      * :const:`tlp.TulipFontAwesome.Barcode`
      * :const:`tlp.TulipFontAwesome.Bars`
      * :const:`tlp.TulipFontAwesome.Bed`
      * :const:`tlp.TulipFontAwesome.Beer`
      * :const:`tlp.TulipFontAwesome.Behance`
      * :const:`tlp.TulipFontAwesome.BehanceSquare`
      * :const:`tlp.TulipFontAwesome.Bell`
      * :const:`tlp.TulipFontAwesome.BellO`
      * :const:`tlp.TulipFontAwesome.BellSlash`
      * :const:`tlp.TulipFontAwesome.BellSlashO`
      * :const:`tlp.TulipFontAwesome.Bicycle`
      * :const:`tlp.TulipFontAwesome.Binoculars`
      * :const:`tlp.TulipFontAwesome.BirthdayCake`
      * :const:`tlp.TulipFontAwesome.Bitbucket`
      * :const:`tlp.TulipFontAwesome.BitbucketSquare`
      * :const:`tlp.TulipFontAwesome.Bitcoin`
      * :const:`tlp.TulipFontAwesome.Bold`
      * :const:`tlp.TulipFontAwesome.Bolt`
      * :const:`tlp.TulipFontAwesome.Bomb`
      * :const:`tlp.TulipFontAwesome.Book`
      * :const:`tlp.TulipFontAwesome.Bookmark`
      * :const:`tlp.TulipFontAwesome.BookmarkO`
      * :const:`tlp.TulipFontAwesome.Briefcase`
      * :const:`tlp.TulipFontAwesome.Btc`
      * :const:`tlp.TulipFontAwesome.Bug`
      * :const:`tlp.TulipFontAwesome.Building`
      * :const:`tlp.TulipFontAwesome.BuildingO`
      * :const:`tlp.TulipFontAwesome.Bullhorn`
      * :const:`tlp.TulipFontAwesome.Bullseye`
      * :const:`tlp.TulipFontAwesome.Bus`
      * :const:`tlp.TulipFontAwesome.Buysellads`
      * :const:`tlp.TulipFontAwesome.Cab`
      * :const:`tlp.TulipFontAwesome.Calculator`
      * :const:`tlp.TulipFontAwesome.Calendar`
      * :const:`tlp.TulipFontAwesome.CalendarO`
      * :const:`tlp.TulipFontAwesome.Camera`
      * :const:`tlp.TulipFontAwesome.CameraRetro`
      * :const:`tlp.TulipFontAwesome.Car`
      * :const:`tlp.TulipFontAwesome.CaretDown`
      * :const:`tlp.TulipFontAwesome.CaretLeft`
      * :const:`tlp.TulipFontAwesome.CaretRight`
      * :const:`tlp.TulipFontAwesome.CaretSquareODown`
      * :const:`tlp.TulipFontAwesome.CaretSquareOLeft`
      * :const:`tlp.TulipFontAwesome.CaretSquareORight`
      * :const:`tlp.TulipFontAwesome.CaretSquareOUp`
      * :const:`tlp.TulipFontAwesome.CaretUp`
      * :const:`tlp.TulipFontAwesome.CartArrowDown`
      * :const:`tlp.TulipFontAwesome.CartPlus`
      * :const:`tlp.TulipFontAwesome.Cc`
      * :const:`tlp.TulipFontAwesome.CcAmex`
      * :const:`tlp.TulipFontAwesome.CcDiscover`
      * :const:`tlp.TulipFontAwesome.CcMastercard`
      * :const:`tlp.TulipFontAwesome.CcPaypal`
      * :const:`tlp.TulipFontAwesome.CcStripe`
      * :const:`tlp.TulipFontAwesome.CcVisa`
      * :const:`tlp.TulipFontAwesome.Certificate`
      * :const:`tlp.TulipFontAwesome.Chain`
      * :const:`tlp.TulipFontAwesome.ChainBroken`
      * :const:`tlp.TulipFontAwesome.Check`
      * :const:`tlp.TulipFontAwesome.CheckCircle`
      * :const:`tlp.TulipFontAwesome.CheckCircleO`
      * :const:`tlp.TulipFontAwesome.CheckSquare`
      * :const:`tlp.TulipFontAwesome.CheckSquareO`
      * :const:`tlp.TulipFontAwesome.ChevronCircleDown`
      * :const:`tlp.TulipFontAwesome.ChevronCircleLeft`
      * :const:`tlp.TulipFontAwesome.ChevronCircleRight`
      * :const:`tlp.TulipFontAwesome.ChevronCircleUp`
      * :const:`tlp.TulipFontAwesome.ChevronDown`
      * :const:`tlp.TulipFontAwesome.ChevronLeft`
      * :const:`tlp.TulipFontAwesome.ChevronRight`
      * :const:`tlp.TulipFontAwesome.ChevronUp`
      * :const:`tlp.TulipFontAwesome.Child`
      * :const:`tlp.TulipFontAwesome.Circle`
      * :const:`tlp.TulipFontAwesome.CircleO`
      * :const:`tlp.TulipFontAwesome.CircleONotch`
      * :const:`tlp.TulipFontAwesome.CircleThin`
      * :const:`tlp.TulipFontAwesome.Clipboard`
      * :const:`tlp.TulipFontAwesome.ClockO`
      * :const:`tlp.TulipFontAwesome.Close`
      * :const:`tlp.TulipFontAwesome.Cloud`
      * :const:`tlp.TulipFontAwesome.CloudDownload`
      * :const:`tlp.TulipFontAwesome.CloudUpload`
      * :const:`tlp.TulipFontAwesome.Cny`
      * :const:`tlp.TulipFontAwesome.Code`
      * :const:`tlp.TulipFontAwesome.CodeFork`
      * :const:`tlp.TulipFontAwesome.Codepen`
      * :const:`tlp.TulipFontAwesome.Coffee`
      * :const:`tlp.TulipFontAwesome.Cog`
      * :const:`tlp.TulipFontAwesome.Cogs`
      * :const:`tlp.TulipFontAwesome.Columns`
      * :const:`tlp.TulipFontAwesome.Comment`
      * :const:`tlp.TulipFontAwesome.CommentO`
      * :const:`tlp.TulipFontAwesome.Comments`
      * :const:`tlp.TulipFontAwesome.CommentsO`
      * :const:`tlp.TulipFontAwesome.Compass`
      * :const:`tlp.TulipFontAwesome.Compress`
      * :const:`tlp.TulipFontAwesome.Connectdevelop`
      * :const:`tlp.TulipFontAwesome.Copy`
      * :const:`tlp.TulipFontAwesome.Copyright`
      * :const:`tlp.TulipFontAwesome.CreditCard`
      * :const:`tlp.TulipFontAwesome.Crop`
      * :const:`tlp.TulipFontAwesome.Crosshairs`
      * :const:`tlp.TulipFontAwesome.Css3`
      * :const:`tlp.TulipFontAwesome.Cube`
      * :const:`tlp.TulipFontAwesome.Cubes`
      * :const:`tlp.TulipFontAwesome.Cut`
      * :const:`tlp.TulipFontAwesome.Cutlery`
      * :const:`tlp.TulipFontAwesome.Dashboard`
      * :const:`tlp.TulipFontAwesome.Dashcube`
      * :const:`tlp.TulipFontAwesome.Database`
      * :const:`tlp.TulipFontAwesome.Dedent`
      * :const:`tlp.TulipFontAwesome.Delicious`
      * :const:`tlp.TulipFontAwesome.Desktop`
      * :const:`tlp.TulipFontAwesome.Deviantart`
      * :const:`tlp.TulipFontAwesome.Diamond`
      * :const:`tlp.TulipFontAwesome.Digg`
      * :const:`tlp.TulipFontAwesome.Dollar`
      * :const:`tlp.TulipFontAwesome.DotCircleO`
      * :const:`tlp.TulipFontAwesome.Download`
      * :const:`tlp.TulipFontAwesome.Dribbble`
      * :const:`tlp.TulipFontAwesome.Dropbox`
      * :const:`tlp.TulipFontAwesome.Drupal`
      * :const:`tlp.TulipFontAwesome.Edit`
      * :const:`tlp.TulipFontAwesome.Eject`
      * :const:`tlp.TulipFontAwesome.EllipsisH`
      * :const:`tlp.TulipFontAwesome.EllipsisV`
      * :const:`tlp.TulipFontAwesome.Empire`
      * :const:`tlp.TulipFontAwesome.Envelope`
      * :const:`tlp.TulipFontAwesome.EnvelopeO`
      * :const:`tlp.TulipFontAwesome.EnvelopeSquare`
      * :const:`tlp.TulipFontAwesome.Eraser`
      * :const:`tlp.TulipFontAwesome.Eur`
      * :const:`tlp.TulipFontAwesome.Euro`
      * :const:`tlp.TulipFontAwesome.Exchange`
      * :const:`tlp.TulipFontAwesome.Exclamation`
      * :const:`tlp.TulipFontAwesome.ExclamationCircle`
      * :const:`tlp.TulipFontAwesome.ExclamationTriangle`
      * :const:`tlp.TulipFontAwesome.Expand`
      * :const:`tlp.TulipFontAwesome.ExternalLink`
      * :const:`tlp.TulipFontAwesome.ExternalLinkSquare`
      * :const:`tlp.TulipFontAwesome.Eye`
      * :const:`tlp.TulipFontAwesome.EyeSlash`
      * :const:`tlp.TulipFontAwesome.Eyedropper`
      * :const:`tlp.TulipFontAwesome.Facebook`
      * :const:`tlp.TulipFontAwesome.FacebookF`
      * :const:`tlp.TulipFontAwesome.FacebookOfficial`
      * :const:`tlp.TulipFontAwesome.FacebookSquare`
      * :const:`tlp.TulipFontAwesome.FastBackward`
      * :const:`tlp.TulipFontAwesome.FastForward`
      * :const:`tlp.TulipFontAwesome.Fax`
      * :const:`tlp.TulipFontAwesome.Female`
      * :const:`tlp.TulipFontAwesome.FighterJet`
      * :const:`tlp.TulipFontAwesome.File`
      * :const:`tlp.TulipFontAwesome.FileArchiveO`
      * :const:`tlp.TulipFontAwesome.FileAudioO`
      * :const:`tlp.TulipFontAwesome.FileCodeO`
      * :const:`tlp.TulipFontAwesome.FileExcelO`
      * :const:`tlp.TulipFontAwesome.FileImageO`
      * :const:`tlp.TulipFontAwesome.FileMovieO`
      * :const:`tlp.TulipFontAwesome.FileO`
      * :const:`tlp.TulipFontAwesome.FilePdfO`
      * :const:`tlp.TulipFontAwesome.FilePhotoO`
      * :const:`tlp.TulipFontAwesome.FilePictureO`
      * :const:`tlp.TulipFontAwesome.FilePowerpointO`
      * :const:`tlp.TulipFontAwesome.FileSoundO`
      * :const:`tlp.TulipFontAwesome.FileText`
      * :const:`tlp.TulipFontAwesome.FileTextO`
      * :const:`tlp.TulipFontAwesome.FileVideoO`
      * :const:`tlp.TulipFontAwesome.FileWordO`
      * :const:`tlp.TulipFontAwesome.FileZipO`
      * :const:`tlp.TulipFontAwesome.FilesO`
      * :const:`tlp.TulipFontAwesome.Film`
      * :const:`tlp.TulipFontAwesome.Filter`
      * :const:`tlp.TulipFontAwesome.Fire`
      * :const:`tlp.TulipFontAwesome.FireExtinguisher`
      * :const:`tlp.TulipFontAwesome.Flag`
      * :const:`tlp.TulipFontAwesome.FlagCheckered`
      * :const:`tlp.TulipFontAwesome.FlagO`
      * :const:`tlp.TulipFontAwesome.Flash`
      * :const:`tlp.TulipFontAwesome.Flask`
      * :const:`tlp.TulipFontAwesome.Flickr`
      * :const:`tlp.TulipFontAwesome.FloppyO`
      * :const:`tlp.TulipFontAwesome.Folder`
      * :const:`tlp.TulipFontAwesome.FolderO`
      * :const:`tlp.TulipFontAwesome.FolderOpen`
      * :const:`tlp.TulipFontAwesome.FolderOpenO`
      * :const:`tlp.TulipFontAwesome.Font`
      * :const:`tlp.TulipFontAwesome.Forumbee`
      * :const:`tlp.TulipFontAwesome.Forward`
      * :const:`tlp.TulipFontAwesome.Foursquare`
      * :const:`tlp.TulipFontAwesome.FrownO`
      * :const:`tlp.TulipFontAwesome.FutbolO`
      * :const:`tlp.TulipFontAwesome.Gamepad`
      * :const:`tlp.TulipFontAwesome.Gavel`
      * :const:`tlp.TulipFontAwesome.Gbp`
      * :const:`tlp.TulipFontAwesome.Ge`
      * :const:`tlp.TulipFontAwesome.Gear`
      * :const:`tlp.TulipFontAwesome.Gears`
      * :const:`tlp.TulipFontAwesome.Genderless`
      * :const:`tlp.TulipFontAwesome.Gift`
      * :const:`tlp.TulipFontAwesome.Git`
      * :const:`tlp.TulipFontAwesome.GitSquare`
      * :const:`tlp.TulipFontAwesome.Github`
      * :const:`tlp.TulipFontAwesome.GithubAlt`
      * :const:`tlp.TulipFontAwesome.GithubSquare`
      * :const:`tlp.TulipFontAwesome.Gittip`
      * :const:`tlp.TulipFontAwesome.Glass`
      * :const:`tlp.TulipFontAwesome.Globe`
      * :const:`tlp.TulipFontAwesome.Google`
      * :const:`tlp.TulipFontAwesome.GooglePlus`
      * :const:`tlp.TulipFontAwesome.GooglePlusSquare`
      * :const:`tlp.TulipFontAwesome.GoogleWallet`
      * :const:`tlp.TulipFontAwesome.GraduationCap`
      * :const:`tlp.TulipFontAwesome.Gratipay`
      * :const:`tlp.TulipFontAwesome.Group`
      * :const:`tlp.TulipFontAwesome.HSquare`
      * :const:`tlp.TulipFontAwesome.HackerNews`
      * :const:`tlp.TulipFontAwesome.HandODown`
      * :const:`tlp.TulipFontAwesome.HandOLeft`
      * :const:`tlp.TulipFontAwesome.HandORight`
      * :const:`tlp.TulipFontAwesome.HandOUp`
      * :const:`tlp.TulipFontAwesome.HddO`
      * :const:`tlp.TulipFontAwesome.Header`
      * :const:`tlp.TulipFontAwesome.Headphones`
      * :const:`tlp.TulipFontAwesome.Heart`
      * :const:`tlp.TulipFontAwesome.HeartO`
      * :const:`tlp.TulipFontAwesome.Heartbeat`
      * :const:`tlp.TulipFontAwesome.History`
      * :const:`tlp.TulipFontAwesome.Home`
      * :const:`tlp.TulipFontAwesome.HospitalO`
      * :const:`tlp.TulipFontAwesome.Hotel`
      * :const:`tlp.TulipFontAwesome.Html5`
      * :const:`tlp.TulipFontAwesome.Ils`
      * :const:`tlp.TulipFontAwesome.Image`
      * :const:`tlp.TulipFontAwesome.Inbox`
      * :const:`tlp.TulipFontAwesome.Indent`
      * :const:`tlp.TulipFontAwesome.Info`
      * :const:`tlp.TulipFontAwesome.InfoCircle`
      * :const:`tlp.TulipFontAwesome.Inr`
      * :const:`tlp.TulipFontAwesome.Instagram`
      * :const:`tlp.TulipFontAwesome.Institution`
      * :const:`tlp.TulipFontAwesome.Ioxhost`
      * :const:`tlp.TulipFontAwesome.Italic`
      * :const:`tlp.TulipFontAwesome.Joomla`
      * :const:`tlp.TulipFontAwesome.Jpy`
      * :const:`tlp.TulipFontAwesome.Jsfiddle`
      * :const:`tlp.TulipFontAwesome.Key`
      * :const:`tlp.TulipFontAwesome.KeyboardO`
      * :const:`tlp.TulipFontAwesome.Krw`
      * :const:`tlp.TulipFontAwesome.Language`
      * :const:`tlp.TulipFontAwesome.Laptop`
      * :const:`tlp.TulipFontAwesome.Lastfm`
      * :const:`tlp.TulipFontAwesome.LastfmSquare`
      * :const:`tlp.TulipFontAwesome.Leaf`
      * :const:`tlp.TulipFontAwesome.Leanpub`
      * :const:`tlp.TulipFontAwesome.Legal`
      * :const:`tlp.TulipFontAwesome.LemonO`
      * :const:`tlp.TulipFontAwesome.LevelDown`
      * :const:`tlp.TulipFontAwesome.LevelUp`
      * :const:`tlp.TulipFontAwesome.LifeBouy`
      * :const:`tlp.TulipFontAwesome.LifeBuoy`
      * :const:`tlp.TulipFontAwesome.LifeRing`
      * :const:`tlp.TulipFontAwesome.LifeSaver`
      * :const:`tlp.TulipFontAwesome.LightbulbO`
      * :const:`tlp.TulipFontAwesome.LineChart`
      * :const:`tlp.TulipFontAwesome.Link`
      * :const:`tlp.TulipFontAwesome.Linkedin`
      * :const:`tlp.TulipFontAwesome.LinkedinSquare`
      * :const:`tlp.TulipFontAwesome.Linux`
      * :const:`tlp.TulipFontAwesome.List`
      * :const:`tlp.TulipFontAwesome.ListAlt`
      * :const:`tlp.TulipFontAwesome.ListOl`
      * :const:`tlp.TulipFontAwesome.ListUl`
      * :const:`tlp.TulipFontAwesome.LocationArrow`
      * :const:`tlp.TulipFontAwesome.Lock`
      * :const:`tlp.TulipFontAwesome.LongArrowDown`
      * :const:`tlp.TulipFontAwesome.LongArrowLeft`
      * :const:`tlp.TulipFontAwesome.LongArrowRight`
      * :const:`tlp.TulipFontAwesome.LongArrowUp`
      * :const:`tlp.TulipFontAwesome.Magic`
      * :const:`tlp.TulipFontAwesome.Magnet`
      * :const:`tlp.TulipFontAwesome.MailForward`
      * :const:`tlp.TulipFontAwesome.MailReply`
      * :const:`tlp.TulipFontAwesome.MailReplyAll`
      * :const:`tlp.TulipFontAwesome.Male`
      * :const:`tlp.TulipFontAwesome.MapMarker`
      * :const:`tlp.TulipFontAwesome.Mars`
      * :const:`tlp.TulipFontAwesome.MarsDouble`
      * :const:`tlp.TulipFontAwesome.MarsStroke`
      * :const:`tlp.TulipFontAwesome.MarsStrokeH`
      * :const:`tlp.TulipFontAwesome.MarsStrokeV`
      * :const:`tlp.TulipFontAwesome.Maxcdn`
      * :const:`tlp.TulipFontAwesome.Meanpath`
      * :const:`tlp.TulipFontAwesome.Medium`
      * :const:`tlp.TulipFontAwesome.Medkit`
      * :const:`tlp.TulipFontAwesome.MehO`
      * :const:`tlp.TulipFontAwesome.Mercury`
      * :const:`tlp.TulipFontAwesome.Microphone`
      * :const:`tlp.TulipFontAwesome.MicrophoneSlash`
      * :const:`tlp.TulipFontAwesome.Minus`
      * :const:`tlp.TulipFontAwesome.MinusCircle`
      * :const:`tlp.TulipFontAwesome.MinusSquare`
      * :const:`tlp.TulipFontAwesome.MinusSquareO`
      * :const:`tlp.TulipFontAwesome.Mobile`
      * :const:`tlp.TulipFontAwesome.MobilePhone`
      * :const:`tlp.TulipFontAwesome.Money`
      * :const:`tlp.TulipFontAwesome.MoonO`
      * :const:`tlp.TulipFontAwesome.MortarBoard`
      * :const:`tlp.TulipFontAwesome.Motorcycle`
      * :const:`tlp.TulipFontAwesome.Music`
      * :const:`tlp.TulipFontAwesome.Navicon`
      * :const:`tlp.TulipFontAwesome.Neuter`
      * :const:`tlp.TulipFontAwesome.NewspaperO`
      * :const:`tlp.TulipFontAwesome.Openid`
      * :const:`tlp.TulipFontAwesome.Outdent`
      * :const:`tlp.TulipFontAwesome.Pagelines`
      * :const:`tlp.TulipFontAwesome.PaintBrush`
      * :const:`tlp.TulipFontAwesome.PaperPlane`
      * :const:`tlp.TulipFontAwesome.PaperPlaneO`
      * :const:`tlp.TulipFontAwesome.Paperclip`
      * :const:`tlp.TulipFontAwesome.Paragraph`
      * :const:`tlp.TulipFontAwesome.Paste`
      * :const:`tlp.TulipFontAwesome.Pause`
      * :const:`tlp.TulipFontAwesome.Paw`
      * :const:`tlp.TulipFontAwesome.Paypal`
      * :const:`tlp.TulipFontAwesome.Pencil`
      * :const:`tlp.TulipFontAwesome.PencilSquare`
      * :const:`tlp.TulipFontAwesome.PencilSquareO`
      * :const:`tlp.TulipFontAwesome.Phone`
      * :const:`tlp.TulipFontAwesome.PhoneSquare`
      * :const:`tlp.TulipFontAwesome.Photo`
      * :const:`tlp.TulipFontAwesome.PictureO`
      * :const:`tlp.TulipFontAwesome.PieChart`
      * :const:`tlp.TulipFontAwesome.PiedPiper`
      * :const:`tlp.TulipFontAwesome.PiedPiperAlt`
      * :const:`tlp.TulipFontAwesome.Pinterest`
      * :const:`tlp.TulipFontAwesome.PinterestP`
      * :const:`tlp.TulipFontAwesome.PinterestSquare`
      * :const:`tlp.TulipFontAwesome.Plane`
      * :const:`tlp.TulipFontAwesome.Play`
      * :const:`tlp.TulipFontAwesome.PlayCircle`
      * :const:`tlp.TulipFontAwesome.PlayCircleO`
      * :const:`tlp.TulipFontAwesome.Plug`
      * :const:`tlp.TulipFontAwesome.Plus`
      * :const:`tlp.TulipFontAwesome.PlusCircle`
      * :const:`tlp.TulipFontAwesome.PlusSquare`
      * :const:`tlp.TulipFontAwesome.PlusSquareO`
      * :const:`tlp.TulipFontAwesome.PowerOff`
      * :const:`tlp.TulipFontAwesome.Print`
      * :const:`tlp.TulipFontAwesome.PuzzlePiece`
      * :const:`tlp.TulipFontAwesome.Qq`
      * :const:`tlp.TulipFontAwesome.Qrcode`
      * :const:`tlp.TulipFontAwesome.Question`
      * :const:`tlp.TulipFontAwesome.QuestionCircle`
      * :const:`tlp.TulipFontAwesome.QuoteLeft`
      * :const:`tlp.TulipFontAwesome.QuoteRight`
      * :const:`tlp.TulipFontAwesome.Ra`
      * :const:`tlp.TulipFontAwesome.Random`
      * :const:`tlp.TulipFontAwesome.Rebel`
      * :const:`tlp.TulipFontAwesome.Recycle`
      * :const:`tlp.TulipFontAwesome.Reddit`
      * :const:`tlp.TulipFontAwesome.RedditSquare`
      * :const:`tlp.TulipFontAwesome.Refresh`
      * :const:`tlp.TulipFontAwesome.Remove`
      * :const:`tlp.TulipFontAwesome.Renren`
      * :const:`tlp.TulipFontAwesome.Reorder`
      * :const:`tlp.TulipFontAwesome.Repeat`
      * :const:`tlp.TulipFontAwesome.Reply`
      * :const:`tlp.TulipFontAwesome.ReplyAll`
      * :const:`tlp.TulipFontAwesome.Retweet`
      * :const:`tlp.TulipFontAwesome.Rmb`
      * :const:`tlp.TulipFontAwesome.Road`
      * :const:`tlp.TulipFontAwesome.Rocket`
      * :const:`tlp.TulipFontAwesome.RotateLeft`
      * :const:`tlp.TulipFontAwesome.RotateRight`
      * :const:`tlp.TulipFontAwesome.Rouble`
      * :const:`tlp.TulipFontAwesome.Rss`
      * :const:`tlp.TulipFontAwesome.RssSquare`
      * :const:`tlp.TulipFontAwesome.Rub`
      * :const:`tlp.TulipFontAwesome.Ruble`
      * :const:`tlp.TulipFontAwesome.Rupee`
      * :const:`tlp.TulipFontAwesome.Save`
      * :const:`tlp.TulipFontAwesome.Scissors`
      * :const:`tlp.TulipFontAwesome.Search`
      * :const:`tlp.TulipFontAwesome.SearchMinus`
      * :const:`tlp.TulipFontAwesome.SearchPlus`
      * :const:`tlp.TulipFontAwesome.Sellsy`
      * :const:`tlp.TulipFontAwesome.Send`
      * :const:`tlp.TulipFontAwesome.SendO`
      * :const:`tlp.TulipFontAwesome.Server`
      * :const:`tlp.TulipFontAwesome.Share`
      * :const:`tlp.TulipFontAwesome.ShareAlt`
      * :const:`tlp.TulipFontAwesome.ShareAltSquare`
      * :const:`tlp.TulipFontAwesome.ShareSquare`
      * :const:`tlp.TulipFontAwesome.ShareSquareO`
      * :const:`tlp.TulipFontAwesome.Shekel`
      * :const:`tlp.TulipFontAwesome.Sheqel`
      * :const:`tlp.TulipFontAwesome.Shield`
      * :const:`tlp.TulipFontAwesome.Ship`
      * :const:`tlp.TulipFontAwesome.Shirtsinbulk`
      * :const:`tlp.TulipFontAwesome.ShoppingCart`
      * :const:`tlp.TulipFontAwesome.SignIn`
      * :const:`tlp.TulipFontAwesome.SignOut`
      * :const:`tlp.TulipFontAwesome.Signal`
      * :const:`tlp.TulipFontAwesome.Simplybuilt`
      * :const:`tlp.TulipFontAwesome.Sitemap`
      * :const:`tlp.TulipFontAwesome.Skyatlas`
      * :const:`tlp.TulipFontAwesome.Skype`
      * :const:`tlp.TulipFontAwesome.Slack`
      * :const:`tlp.TulipFontAwesome.Sliders`
      * :const:`tlp.TulipFontAwesome.Slideshare`
      * :const:`tlp.TulipFontAwesome.SmileO`
      * :const:`tlp.TulipFontAwesome.SoccerBallO`
      * :const:`tlp.TulipFontAwesome.Sort`
      * :const:`tlp.TulipFontAwesome.SortAlphaAsc`
      * :const:`tlp.TulipFontAwesome.SortAlphaDesc`
      * :const:`tlp.TulipFontAwesome.SortAmountAsc`
      * :const:`tlp.TulipFontAwesome.SortAmountDesc`
      * :const:`tlp.TulipFontAwesome.SortAsc`
      * :const:`tlp.TulipFontAwesome.SortDesc`
      * :const:`tlp.TulipFontAwesome.SortDown`
      * :const:`tlp.TulipFontAwesome.SortNumericAsc`
      * :const:`tlp.TulipFontAwesome.SortNumericDesc`
      * :const:`tlp.TulipFontAwesome.SortUp`
      * :const:`tlp.TulipFontAwesome.Soundcloud`
      * :const:`tlp.TulipFontAwesome.SpaceShuttle`
      * :const:`tlp.TulipFontAwesome.Spinner`
      * :const:`tlp.TulipFontAwesome.Spoon`
      * :const:`tlp.TulipFontAwesome.Spotify`
      * :const:`tlp.TulipFontAwesome.Square`
      * :const:`tlp.TulipFontAwesome.SquareO`
      * :const:`tlp.TulipFontAwesome.StackExchange`
      * :const:`tlp.TulipFontAwesome.StackOverflow`
      * :const:`tlp.TulipFontAwesome.Star`
      * :const:`tlp.TulipFontAwesome.StarHalf`
      * :const:`tlp.TulipFontAwesome.StarHalfEmpty`
      * :const:`tlp.TulipFontAwesome.StarHalfFull`
      * :const:`tlp.TulipFontAwesome.StarHalfO`
      * :const:`tlp.TulipFontAwesome.StarO`
      * :const:`tlp.TulipFontAwesome.Steam`
      * :const:`tlp.TulipFontAwesome.SteamSquare`
      * :const:`tlp.TulipFontAwesome.StepBackward`
      * :const:`tlp.TulipFontAwesome.StepForward`
      * :const:`tlp.TulipFontAwesome.Stethoscope`
      * :const:`tlp.TulipFontAwesome.Stop`
      * :const:`tlp.TulipFontAwesome.StreetView`
      * :const:`tlp.TulipFontAwesome.Strikethrough`
      * :const:`tlp.TulipFontAwesome.Stumbleupon`
      * :const:`tlp.TulipFontAwesome.StumbleuponCircle`
      * :const:`tlp.TulipFontAwesome.Subscript`
      * :const:`tlp.TulipFontAwesome.Subway`
      * :const:`tlp.TulipFontAwesome.Suitcase`
      * :const:`tlp.TulipFontAwesome.SunO`
      * :const:`tlp.TulipFontAwesome.Superscript`
      * :const:`tlp.TulipFontAwesome.Support`
      * :const:`tlp.TulipFontAwesome.Table`
      * :const:`tlp.TulipFontAwesome.Tablet`
      * :const:`tlp.TulipFontAwesome.Tachometer`
      * :const:`tlp.TulipFontAwesome.Tag`
      * :const:`tlp.TulipFontAwesome.Tags`
      * :const:`tlp.TulipFontAwesome.Tasks`
      * :const:`tlp.TulipFontAwesome.Taxi`
      * :const:`tlp.TulipFontAwesome.TencentWeibo`
      * :const:`tlp.TulipFontAwesome.Terminal`
      * :const:`tlp.TulipFontAwesome.TextHeight`
      * :const:`tlp.TulipFontAwesome.TextWidth`
      * :const:`tlp.TulipFontAwesome.Th`
      * :const:`tlp.TulipFontAwesome.ThLarge`
      * :const:`tlp.TulipFontAwesome.ThList`
      * :const:`tlp.TulipFontAwesome.ThumbTack`
      * :const:`tlp.TulipFontAwesome.ThumbsDown`
      * :const:`tlp.TulipFontAwesome.ThumbsODown`
      * :const:`tlp.TulipFontAwesome.ThumbsOUp`
      * :const:`tlp.TulipFontAwesome.ThumbsUp`
      * :const:`tlp.TulipFontAwesome.Ticket`
      * :const:`tlp.TulipFontAwesome.Times`
      * :const:`tlp.TulipFontAwesome.TimesCircle`
      * :const:`tlp.TulipFontAwesome.TimesCircleO`
      * :const:`tlp.TulipFontAwesome.Tint`
      * :const:`tlp.TulipFontAwesome.ToggleDown`
      * :const:`tlp.TulipFontAwesome.ToggleLeft`
      * :const:`tlp.TulipFontAwesome.ToggleOff`
      * :const:`tlp.TulipFontAwesome.ToggleOn`
      * :const:`tlp.TulipFontAwesome.ToggleRight`
      * :const:`tlp.TulipFontAwesome.ToggleUp`
      * :const:`tlp.TulipFontAwesome.Train`
      * :const:`tlp.TulipFontAwesome.Transgender`
      * :const:`tlp.TulipFontAwesome.TransgenderAlt`
      * :const:`tlp.TulipFontAwesome.Trash`
      * :const:`tlp.TulipFontAwesome.TrashO`
      * :const:`tlp.TulipFontAwesome.Tree`
      * :const:`tlp.TulipFontAwesome.Trello`
      * :const:`tlp.TulipFontAwesome.Trophy`
      * :const:`tlp.TulipFontAwesome.Truck`
      * :const:`tlp.TulipFontAwesome.Try`
      * :const:`tlp.TulipFontAwesome.Tty`
      * :const:`tlp.TulipFontAwesome.Tumblr`
      * :const:`tlp.TulipFontAwesome.TumblrSquare`
      * :const:`tlp.TulipFontAwesome.TurkishLira`
      * :const:`tlp.TulipFontAwesome.Twitch`
      * :const:`tlp.TulipFontAwesome.Twitter`
      * :const:`tlp.TulipFontAwesome.TwitterSquare`
      * :const:`tlp.TulipFontAwesome.Umbrella`
      * :const:`tlp.TulipFontAwesome.Underline`
      * :const:`tlp.TulipFontAwesome.Undo`
      * :const:`tlp.TulipFontAwesome.University`
      * :const:`tlp.TulipFontAwesome.Unlink`
      * :const:`tlp.TulipFontAwesome.Unlock`
      * :const:`tlp.TulipFontAwesome.UnlockAlt`
      * :const:`tlp.TulipFontAwesome.Unsorted`
      * :const:`tlp.TulipFontAwesome.Upload`
      * :const:`tlp.TulipFontAwesome.Usd`
      * :const:`tlp.TulipFontAwesome.User`
      * :const:`tlp.TulipFontAwesome.UserMd`
      * :const:`tlp.TulipFontAwesome.UserPlus`
      * :const:`tlp.TulipFontAwesome.UserSecret`
      * :const:`tlp.TulipFontAwesome.UserTimes`
      * :const:`tlp.TulipFontAwesome.Users`
      * :const:`tlp.TulipFontAwesome.Venus`
      * :const:`tlp.TulipFontAwesome.VenusDouble`
      * :const:`tlp.TulipFontAwesome.VenusMars`
      * :const:`tlp.TulipFontAwesome.Viacoin`
      * :const:`tlp.TulipFontAwesome.VideoCamera`
      * :const:`tlp.TulipFontAwesome.VimeoSquare`
      * :const:`tlp.TulipFontAwesome.Vine`
      * :const:`tlp.TulipFontAwesome.Vk`
      * :const:`tlp.TulipFontAwesome.VolumeDown`
      * :const:`tlp.TulipFontAwesome.VolumeOff`
      * :const:`tlp.TulipFontAwesome.VolumeUp`
      * :const:`tlp.TulipFontAwesome.Warning`
      * :const:`tlp.TulipFontAwesome.Wechat`
      * :const:`tlp.TulipFontAwesome.Weibo`
      * :const:`tlp.TulipFontAwesome.Weixin`
      * :const:`tlp.TulipFontAwesome.Whatsapp`
      * :const:`tlp.TulipFontAwesome.Wheelchair`
      * :const:`tlp.TulipFontAwesome.Wifi`
      * :const:`tlp.TulipFontAwesome.Windows`
      * :const:`tlp.TulipFontAwesome.Won`
      * :const:`tlp.TulipFontAwesome.Wordpress`
      * :const:`tlp.TulipFontAwesome.Wrench`
      * :const:`tlp.TulipFontAwesome.Xing`
      * :const:`tlp.TulipFontAwesome.XingSquare`
      * :const:`tlp.TulipFontAwesome.Yahoo`
      * :const:`tlp.TulipFontAwesome.Yelp`
      * :const:`tlp.TulipFontAwesome.Yen`
      * :const:`tlp.TulipFontAwesome.Youtube`
      * :const:`tlp.TulipFontAwesome.YoutubePlay`
      * :const:`tlp.TulipFontAwesome.YoutubeSquare`

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

};

};
