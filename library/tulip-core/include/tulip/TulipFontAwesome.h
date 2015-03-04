/*
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

#ifndef TULIPFONTAWESOME_H
#define TULIPFONTAWESOME_H

#include <tulip/tulipconf.h>

#include <vector>

namespace tlp {

/**
 * @brief Helper class for the configuration of a Font Awesome glyph.
 *
 * Font Awesome is a free iconic font by Dave Gandy (see http://fontawesome.io)
 * offering more than 500 customizable scalable vector icons.
 *
 * A glyph has been added to Tulip enabling to use these great icons
 * as nodes and edges extremities shapes.
 *
 * That class offers utility functions and supported icons names constants.
 *
 * To set a node glyph as a Font Awesome icon, the Font Awesome glyph must be associated
 * to the node through the modification of the "viewShape" integer property attached to the graph.
 * The name of the icon to use must then be set in the "viewFontAwesomeIcon" string property.
 * As an example, the following code snippet activates the Font Awesome glyph for all nodes
 * and sets the "user" icon.
 *
 * @code
 *    // graph is a pointer to a tlp::Graph object
 *    tlp::IntegerProperty *viewShape = graph->getProperty<tlp::IntegerProperty>("viewShape");
 *    tlp::StringProperty *viewFontAwesomeIcon = graph->getProperty<tlp::StringProperty>("viewFontAwesomeIcon");
 *
 *    // sets the Font Awesome glyph on all nodes
 *    viewShape->setAllNodeValue(tlp::NodeShape::FontAwesomeIcon);
 *    // sets the "user" glyph for all nodes
 *    viewFontAwesomeIcon->setAllNodeValue(tlp::TulipFontAwesome::User);
 * @endcode
 **/

class TLP_SCOPE TulipFontAwesome {

public:

  /**
   * Returns the location of the Font Awesome ttf file bundled with Tulip
   */
  static std::string getFontAwesomeTrueTypeFileLocation();

  /**
   * Returns the list of supported Font Awesome icons names
   */
  static const std::vector<std::string> &getSupportedFontAwesomeIcons();

  /**
   * Checks if the provided Font Awesome icon name is supported
   * @param iconName the name of the icon to check support
   */
  static bool isFontAwesomeIconSupported(const std::string &iconName);

  /**
   * Returns the Unicode code point associated to an icon name
   * @param iconName the name of the icon to get the codepoint
   */
  static unsigned int getFontAwesomeIconCodePoint(const std::string &iconName);

  /**
   * Returns an UTF-8 encoded string of a Font Awesome icon
   * @param iconName a Font Awesome icon name
   * @return
   */
  static std::string getFontAwesomeIconUtf8String(const std::string &iconName);

  // static constants for the Font Awesome icons names
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

}

#endif // TULIPFONTAWESOME_H

