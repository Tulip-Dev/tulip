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
  static bool isFontAwesomeIconSupported(const std::string& iconName);

  /**
   * Returns the Unicode code point associated to an icon name
   * @param iconName the name of the icon to get the codepoint
   */
  static unsigned int getFontAwesomeIconCodePoint(const std::string& iconName);

  /**
   * Returns an UTF-8 encoded string of a Font Awesome icon
   * @param iconName a Font Awesome icon name
   * @return
   */
  static std::string getFontAwesomeIconUtf8String(const std::string& iconName);

  // static constants for the Font Awesome 4.3 icons names
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
  static const char* Fire;
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

  // static constants for the new icons added in Font Awesome 4.4
  static const char* Yc;
  static const char* YCombinator;
  static const char* OptinMonster;
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

  // static constants for the new icons added in Font Awesome 4.5
  static const char * Bluetooth;
  static const char * BluetoothB;
  static const char * Codiepie;
  static const char * CreditCardAlt;
  static const char * Edge;
  static const char * FortAwesome;
  static const char * Hashtag;
  static const char * Mixcloud;
  static const char * Modx;
  static const char * PauseCircle;
  static const char * PauseCircleO;
  static const char * Percent;
  static const char * ProductHunt;
  static const char * RedditAlien;
  static const char * Scribd;
  static const char * ShoppingBag;
  static const char * ShoppingBasket;
  static const char * StopCircle;
  static const char * StopCircleO;
  static const char * Usb;

  // static constants for the new icons added in Font Awesome 4.6
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

/// A list of all icon-names with the codepoint (unicode-value) on the right
/// You can use the names on the page  http://fortawesome.github.io/Font-Awesome/design.html
class fa {

public:

  enum iconCodePoint {

    fa_500px             = 0xf26e,
    adjust               = 0xf042,
    adn                  = 0xf170,
    aligncenter          = 0xf037,
    alignjustify         = 0xf039,
    alignleft            = 0xf036,
    alignright           = 0xf038,
    amazon               = 0xf270,
    ambulance            = 0xf0f9,
    anchor               = 0xf13d,
    android              = 0xf17b,
    angellist            = 0xf209,
    angledoubledown      = 0xf103,
    angledoubleleft      = 0xf100,
    angledoubleright     = 0xf101,
    angledoubleup        = 0xf102,
    angledown            = 0xf107,
    angleleft            = 0xf104,
    angleright           = 0xf105,
    angleup              = 0xf106,
    apple                = 0xf179,
    archive              = 0xf187,
    areachart            = 0xf1fe,
    arrowcircledown      = 0xf0ab,
    arrowcircleleft      = 0xf0a8,
    arrowcircleodown     = 0xf01a,
    arrowcircleoleft     = 0xf190,
    arrowcircleoright    = 0xf18e,
    arrowcircleoup       = 0xf01b,
    arrowcircleright     = 0xf0a9,
    arrowcircleup        = 0xf0aa,
    arrowdown            = 0xf063,
    arrowleft            = 0xf060,
    arrowright           = 0xf061,
    arrowup              = 0xf062,
    arrows               = 0xf047,
    arrowsalt            = 0xf0b2,
    arrowsh              = 0xf07e,
    arrowsv              = 0xf07d,
    asterisk             = 0xf069,
    at                   = 0xf1fa,
    automobile           = 0xf1b9,
    backward             = 0xf04a,
    balancescale         = 0xf24e,
    ban                  = 0xf05e,
    bank                 = 0xf19c,
    barchart             = 0xf080,
    barcharto            = 0xf080,
    barcode              = 0xf02a,
    bars                 = 0xf0c9,
    battery0             = 0xf244,
    battery1             = 0xf243,
    battery2             = 0xf242,
    battery3             = 0xf241,
    battery4             = 0xf240,
    batteryempty         = 0xf244,
    batteryfull          = 0xf240,
    batteryhalf          = 0xf242,
    batteryquarter       = 0xf243,
    batterythreequarters = 0xf241,
    bed                  = 0xf236,
    beer                 = 0xf0fc,
    behance              = 0xf1b4,
    behancesquare        = 0xf1b5,
    bell                 = 0xf0f3,
    bello                = 0xf0a2,
    bellslash            = 0xf1f6,
    bellslasho           = 0xf1f7,
    bicycle              = 0xf206,
    binoculars           = 0xf1e5,
    birthdaycake         = 0xf1fd,
    bitbucket            = 0xf171,
    bitbucketsquare      = 0xf172,
    bitcoin              = 0xf15a,
    blacktie             = 0xf27e,
    bold                 = 0xf032,
    bolt                 = 0xf0e7,
    bomb                 = 0xf1e2,
    book                 = 0xf02d,
    bookmark             = 0xf02e,
    bookmarko            = 0xf097,
    briefcase            = 0xf0b1,
    btc                  = 0xf15a,
    bug                  = 0xf188,
    building             = 0xf1ad,
    buildingo            = 0xf0f7,
    bullhorn             = 0xf0a1,
    bullseye             = 0xf140,
    bus                  = 0xf207,
    buysellads           = 0xf20d,
    cab                  = 0xf1ba,
    calculator           = 0xf1ec,
    calendar             = 0xf073,
    calendarchecko       = 0xf274,
    calendarminuso       = 0xf272,
    calendaro            = 0xf133,
    calendarpluso        = 0xf271,
    calendartimeso       = 0xf273,
    camera               = 0xf030,
    cameraretro          = 0xf083,
    car                  = 0xf1b9,
    caretdown            = 0xf0d7,
    caretleft            = 0xf0d9,
    caretright           = 0xf0da,
    caretsquareodown     = 0xf150,
    caretsquareoleft     = 0xf191,
    caretsquareoright    = 0xf152,
    caretsquareoup       = 0xf151,
    caretup              = 0xf0d8,
    cartarrowdown        = 0xf218,
    cartplus             = 0xf217,
    cc                   = 0xf20a,
    ccamex               = 0xf1f3,
    ccdinersclub         = 0xf24c,
    ccdiscover           = 0xf1f2,
    ccjcb                = 0xf24b,
    ccmastercard         = 0xf1f1,
    ccpaypal             = 0xf1f4,
    ccstripe             = 0xf1f5,
    ccvisa               = 0xf1f0,
    certificate          = 0xf0a3,
    chain                = 0xf0c1,
    chainbroken          = 0xf127,
    check                = 0xf00c,
    checkcircle          = 0xf058,
    checkcircleo         = 0xf05d,
    checksquare          = 0xf14a,
    checksquareo         = 0xf046,
    chevroncircledown    = 0xf13a,
    chevroncircleleft    = 0xf137,
    chevroncircleright   = 0xf138,
    chevroncircleup      = 0xf139,
    chevrondown          = 0xf078,
    chevronleft          = 0xf053,
    chevronright         = 0xf054,
    chevronup            = 0xf077,
    child                = 0xf1ae,
    chrome               = 0xf268,
    circle               = 0xf111,
    circleo              = 0xf10c,
    circleonotch         = 0xf1ce,
    circlethin           = 0xf1db,
    clipboard            = 0xf0ea,
    clocko               = 0xf017,
    clone                = 0xf24d,
    close                = 0xf00d,
    cloud                = 0xf0c2,
    clouddownload        = 0xf0ed,
    cloudupload          = 0xf0ee,
    cny                  = 0xf157,
    code                 = 0xf121,
    codefork             = 0xf126,
    codepen              = 0xf1cb,
    coffee               = 0xf0f4,
    cog                  = 0xf013,
    cogs                 = 0xf085,
    columns              = 0xf0db,
    comment              = 0xf075,
    commento             = 0xf0e5,
    commenting           = 0xf27a,
    commentingo          = 0xf27b,
    comments             = 0xf086,
    commentso            = 0xf0e6,
    compass              = 0xf14e,
    compress             = 0xf066,
    connectdevelop       = 0xf20e,
    contao               = 0xf26d,
    copy                 = 0xf0c5,
    copyright            = 0xf1f9,
    creativecommons      = 0xf25e,
    creditcard           = 0xf09d,
    crop                 = 0xf125,
    crosshairs           = 0xf05b,
    css3                 = 0xf13c,
    cube                 = 0xf1b2,
    cubes                = 0xf1b3,
    cut                  = 0xf0c4,
    cutlery              = 0xf0f5,
    dashboard            = 0xf0e4,
    dashcube             = 0xf210,
    database             = 0xf1c0,
    dedent               = 0xf03b,
    delicious            = 0xf1a5,
    desktop              = 0xf108,
    deviantart           = 0xf1bd,
    diamond              = 0xf219,
    digg                 = 0xf1a6,
    dollar               = 0xf155,
    dotcircleo           = 0xf192,
    download             = 0xf019,
    dribbble             = 0xf17d,
    dropbox              = 0xf16b,
    drupal               = 0xf1a9,
    edit                 = 0xf044,
    eject                = 0xf052,
    ellipsish            = 0xf141,
    ellipsisv            = 0xf142,
    empire               = 0xf1d1,
    envelope             = 0xf0e0,
    envelopeo            = 0xf003,
    envelopesquare       = 0xf199,
    eraser               = 0xf12d,
    eur                  = 0xf153,
    euro                 = 0xf153,
    exchange             = 0xf0ec,
    exclamation          = 0xf12a,
    exclamationcircle    = 0xf06a,
    exclamationtriangle  = 0xf071,
    expand               = 0xf065,
    expeditedssl         = 0xf23e,
    externallink         = 0xf08e,
    externallinksquare   = 0xf14c,
    eye                  = 0xf06e,
    eyeslash             = 0xf070,
    eyedropper           = 0xf1fb,
    facebook             = 0xf09a,
    facebookf            = 0xf09a,
    facebookofficial     = 0xf230,
    facebooksquare       = 0xf082,
    fastbackward         = 0xf049,
    fastforward          = 0xf050,
    fax                  = 0xf1ac,
    feed                 = 0xf09e,
    female               = 0xf182,
    fighterjet           = 0xf0fb,
    file                 = 0xf15b,
    filearchiveo         = 0xf1c6,
    fileaudioo           = 0xf1c7,
    filecodeo            = 0xf1c9,
    fileexcelo           = 0xf1c3,
    fileimageo           = 0xf1c5,
    filemovieo           = 0xf1c8,
    fileo                = 0xf016,
    filepdfo             = 0xf1c1,
    filephotoo           = 0xf1c5,
    filepictureo         = 0xf1c5,
    filepowerpointo      = 0xf1c4,
    filesoundo           = 0xf1c7,
    filetext             = 0xf15c,
    filetexto            = 0xf0f6,
    filevideoo           = 0xf1c8,
    filewordo            = 0xf1c2,
    filezipo             = 0xf1c6,
    fileso               = 0xf0c5,
    film                 = 0xf008,
    filter               = 0xf0b0,
    fire                 = 0xf06d,
    fireextinguisher     = 0xf134,
    firefox              = 0xf269,
    flag                 = 0xf024,
    flagcheckered        = 0xf11e,
    flago                = 0xf11d,
    flash                = 0xf0e7,
    flask                = 0xf0c3,
    flickr               = 0xf16e,
    floppyo              = 0xf0c7,
    folder               = 0xf07b,
    foldero              = 0xf114,
    folderopen           = 0xf07c,
    folderopeno          = 0xf115,
    font                 = 0xf031,
    fonticons            = 0xf280,
    forumbee             = 0xf211,
    forward              = 0xf04e,
    foursquare           = 0xf180,
    frowno               = 0xf119,
    futbolo              = 0xf1e3,
    gamepad              = 0xf11b,
    gavel                = 0xf0e3,
    gbp                  = 0xf154,
    ge                   = 0xf1d1,
    gear                 = 0xf013,
    gears                = 0xf085,
    genderless           = 0xf22d,
    getpocket            = 0xf265,
    gg                   = 0xf260,
    ggcircle             = 0xf261,
    gift                 = 0xf06b,
    git                  = 0xf1d3,
    gitsquare            = 0xf1d2,
    github               = 0xf09b,
    githubalt            = 0xf113,
    githubsquare         = 0xf092,
    gittip               = 0xf184,
    glass                = 0xf000,
    globe                = 0xf0ac,
    google               = 0xf1a0,
    googleplus           = 0xf0d5,
    googleplussquare     = 0xf0d4,
    googlewallet         = 0xf1ee,
    graduationcap        = 0xf19d,
    gratipay             = 0xf184,
    group                = 0xf0c0,
    hsquare              = 0xf0fd,
    hackernews           = 0xf1d4,
    handgrabo            = 0xf255,
    handlizardo          = 0xf258,
    handodown            = 0xf0a7,
    handoleft            = 0xf0a5,
    handoright           = 0xf0a4,
    handoup              = 0xf0a6,
    handpapero           = 0xf256,
    handpeaceo           = 0xf25b,
    handpointero         = 0xf25a,
    handrocko            = 0xf255,
    handscissorso        = 0xf257,
    handspocko           = 0xf259,
    handstopo            = 0xf256,
    hddo                 = 0xf0a0,
    header               = 0xf1dc,
    headphones           = 0xf025,
    heart                = 0xf004,
    hearto               = 0xf08a,
    heartbeat            = 0xf21e,
    history              = 0xf1da,
    home                 = 0xf015,
    hospitalo            = 0xf0f8,
    hotel                = 0xf236,
    hourglass            = 0xf254,
    hourglass1           = 0xf251,
    hourglass2           = 0xf252,
    hourglass3           = 0xf253,
    hourglassend         = 0xf253,
    hourglasshalf        = 0xf252,
    hourglasso           = 0xf250,
    hourglassstart       = 0xf251,
    houzz                = 0xf27c,
    html5                = 0xf13b,
    icursor              = 0xf246,
    ils                  = 0xf20b,
    image                = 0xf03e,
    inbox                = 0xf01c,
    indent               = 0xf03c,
    industry             = 0xf275,
    info                 = 0xf129,
    infocircle           = 0xf05a,
    inr                  = 0xf156,
    instagram            = 0xf16d,
    institution          = 0xf19c,
    internetexplorer     = 0xf26b,
    intersex             = 0xf224,
    ioxhost              = 0xf208,
    italic               = 0xf033,
    joomla               = 0xf1aa,
    jpy                  = 0xf157,
    jsfiddle             = 0xf1cc,
    key                  = 0xf084,
    keyboardo            = 0xf11c,
    krw                  = 0xf159,
    language             = 0xf1ab,
    laptop               = 0xf109,
    lastfm               = 0xf202,
    lastfmsquare         = 0xf203,
    leaf                 = 0xf06c,
    leanpub              = 0xf212,
    legal                = 0xf0e3,
    lemono               = 0xf094,
    leveldown            = 0xf149,
    levelup              = 0xf148,
    lifebouy             = 0xf1cd,
    lifebuoy             = 0xf1cd,
    lifering             = 0xf1cd,
    lifesaver            = 0xf1cd,
    lightbulbo           = 0xf0eb,
    linechart            = 0xf201,
    link                 = 0xf0c1,
    linkedin             = 0xf0e1,
    linkedinsquare       = 0xf08c,
    linux_                = 0xf17c,
    list                 = 0xf03a,
    listalt              = 0xf022,
    listol               = 0xf0cb,
    listul               = 0xf0ca,
    locationarrow        = 0xf124,
    lock                 = 0xf023,
    longarrowdown        = 0xf175,
    longarrowleft        = 0xf177,
    longarrowright       = 0xf178,
    longarrowup          = 0xf176,
    magic                = 0xf0d0,
    magnet               = 0xf076,
    mailforward          = 0xf064,
    mailreply            = 0xf112,
    mailreplyall         = 0xf122,
    male                 = 0xf183,
    map                  = 0xf279,
    mapmarker            = 0xf041,
    mapo                 = 0xf278,
    mappin               = 0xf276,
    mapsigns             = 0xf277,
    mars                 = 0xf222,
    marsdouble           = 0xf227,
    marsstroke           = 0xf229,
    marsstrokeh          = 0xf22b,
    marsstrokev          = 0xf22a,
    maxcdn               = 0xf136,
    meanpath             = 0xf20c,
    medium               = 0xf23a,
    medkit               = 0xf0fa,
    meho                 = 0xf11a,
    mercury              = 0xf223,
    microphone           = 0xf130,
    microphoneslash      = 0xf131,
    minus                = 0xf068,
    minuscircle          = 0xf056,
    minussquare          = 0xf146,
    minussquareo         = 0xf147,
    mobile               = 0xf10b,
    mobilephone          = 0xf10b,
    money                = 0xf0d6,
    moono                = 0xf186,
    mortarboard          = 0xf19d,
    motorcycle           = 0xf21c,
    mousepointer         = 0xf245,
    music                = 0xf001,
    navicon              = 0xf0c9,
    neuter               = 0xf22c,
    newspapero           = 0xf1ea,
    objectgroup          = 0xf247,
    objectungroup        = 0xf248,
    odnoklassniki        = 0xf263,
    odnoklassnikisquare  = 0xf264,
    opencart             = 0xf23d,
    openid               = 0xf19b,
    opera                = 0xf26a,
    optinmonster         = 0xf23c,
    outdent              = 0xf03b,
    pagelines            = 0xf18c,
    paintbrush           = 0xf1fc,
    paperplane           = 0xf1d8,
    paperplaneo          = 0xf1d9,
    paperclip            = 0xf0c6,
    paragraph            = 0xf1dd,
    paste                = 0xf0ea,
    pause                = 0xf04c,
    paw                  = 0xf1b0,
    paypal               = 0xf1ed,
    pencil               = 0xf040,
    pencilsquare         = 0xf14b,
    pencilsquareo        = 0xf044,
    phone                = 0xf095,
    phonesquare          = 0xf098,
    photo                = 0xf03e,
    pictureo             = 0xf03e,
    piechart             = 0xf200,
    piedpiper            = 0xf1a7,
    piedpiperalt         = 0xf1a8,
    pinterest            = 0xf0d2,
    pinterestp           = 0xf231,
    pinterestsquare      = 0xf0d3,
    plane                = 0xf072,
    play                 = 0xf04b,
    playcircle           = 0xf144,
    playcircleo          = 0xf01d,
    plug                 = 0xf1e6,
    plus                 = 0xf067,
    pluscircle           = 0xf055,
    plussquare           = 0xf0fe,
    plussquareo          = 0xf196,
    poweroff             = 0xf011,
    print                = 0xf02f,
    puzzlepiece          = 0xf12e,
    qq                   = 0xf1d6,
    qrcode               = 0xf029,
    question             = 0xf128,
    questioncircle       = 0xf059,
    quoteleft            = 0xf10d,
    quoteright           = 0xf10e,
    ra                   = 0xf1d0,
    random               = 0xf074,
    rebel                = 0xf1d0,
    recycle              = 0xf1b8,
    reddit               = 0xf1a1,
    redditsquare         = 0xf1a2,
    refresh              = 0xf021,
    registered           = 0xf25d,
    remove               = 0xf00d,
    renren               = 0xf18b,
    reorder              = 0xf0c9,
    repeat               = 0xf01e,
    reply                = 0xf112,
    replyall             = 0xf122,
    retweet              = 0xf079,
    rmb                  = 0xf157,
    road                 = 0xf018,
    rocket               = 0xf135,
    rotateleft           = 0xf0e2,
    rotateright          = 0xf01e,
    rouble               = 0xf158,
    rss                  = 0xf09e,
    rsssquare            = 0xf143,
    rub                  = 0xf158,
    ruble                = 0xf158,
    rupee                = 0xf156,
    safari               = 0xf267,
    save                 = 0xf0c7,
    scissors             = 0xf0c4,
    search               = 0xf002,
    searchminus          = 0xf010,
    searchplus           = 0xf00e,
    sellsy               = 0xf213,
    send                 = 0xf1d8,
    sendo                = 0xf1d9,
    server               = 0xf233,
    share                = 0xf064,
    sharealt             = 0xf1e0,
    sharealtsquare       = 0xf1e1,
    sharesquare          = 0xf14d,
    sharesquareo         = 0xf045,
    shekel               = 0xf20b,
    sheqel               = 0xf20b,
    shield               = 0xf132,
    ship                 = 0xf21a,
    shirtsinbulk         = 0xf214,
    shoppingcart         = 0xf07a,
    signin               = 0xf090,
    signout              = 0xf08b,
    signal               = 0xf012,
    simplybuilt          = 0xf215,
    sitemap              = 0xf0e8,
    skyatlas             = 0xf216,
    skype                = 0xf17e,
    slack                = 0xf198,
    sliders              = 0xf1de,
    slideshare           = 0xf1e7,
    smileo               = 0xf118,
    soccerballo          = 0xf1e3,
    sort                 = 0xf0dc,
    sortalphaasc         = 0xf15d,
    sortalphadesc        = 0xf15e,
    sortamountasc        = 0xf160,
    sortamountdesc       = 0xf161,
    sortasc              = 0xf0de,
    sortdesc             = 0xf0dd,
    sortdown             = 0xf0dd,
    sortnumericasc       = 0xf162,
    sortnumericdesc      = 0xf163,
    sortup               = 0xf0de,
    soundcloud           = 0xf1be,
    spaceshuttle         = 0xf197,
    spinner              = 0xf110,
    spoon                = 0xf1b1,
    spotify              = 0xf1bc,
    square               = 0xf0c8,
    squareo              = 0xf096,
    stackexchange        = 0xf18d,
    stackoverflow        = 0xf16c,
    star                 = 0xf005,
    starhalf             = 0xf089,
    starhalfempty        = 0xf123,
    starhalffull         = 0xf123,
    starhalfo            = 0xf123,
    staro                = 0xf006,
    steam                = 0xf1b6,
    steamsquare          = 0xf1b7,
    stepbackward         = 0xf048,
    stepforward          = 0xf051,
    stethoscope          = 0xf0f1,
    stickynote           = 0xf249,
    stickynoteo          = 0xf24a,
    stop                 = 0xf04d,
    streetview           = 0xf21d,
    strikethrough        = 0xf0cc,
    stumbleupon          = 0xf1a4,
    stumbleuponcircle    = 0xf1a3,
    subscript            = 0xf12c,
    subway               = 0xf239,
    suitcase             = 0xf0f2,
    suno                 = 0xf185,
    superscript          = 0xf12b,
    support              = 0xf1cd,
    table                = 0xf0ce,
    tablet               = 0xf10a,
    tachometer           = 0xf0e4,
    tag                  = 0xf02b,
    tags                 = 0xf02c,
    tasks                = 0xf0ae,
    taxi                 = 0xf1ba,
    television           = 0xf26c,
    tencentweibo         = 0xf1d5,
    terminal             = 0xf120,
    textheight           = 0xf034,
    textwidth            = 0xf035,
    th                   = 0xf00a,
    thlarge              = 0xf009,
    thlist               = 0xf00b,
    thumbtack            = 0xf08d,
    thumbsdown           = 0xf165,
    thumbsodown          = 0xf088,
    thumbsoup            = 0xf087,
    thumbsup             = 0xf164,
    ticket               = 0xf145,
    times                = 0xf00d,
    timescircle          = 0xf057,
    timescircleo         = 0xf05c,
    tint                 = 0xf043,
    toggledown           = 0xf150,
    toggleleft           = 0xf191,
    toggleoff            = 0xf204,
    toggleon             = 0xf205,
    toggleright          = 0xf152,
    toggleup             = 0xf151,
    trademark            = 0xf25c,
    train                = 0xf238,
    transgender          = 0xf224,
    transgenderalt       = 0xf225,
    trash                = 0xf1f8,
    trasho               = 0xf014,
    tree                 = 0xf1bb,
    trello               = 0xf181,
    tripadvisor          = 0xf262,
    trophy               = 0xf091,
    truck                = 0xf0d1,
    fa_try               = 0xf195,
    tty                  = 0xf1e4,
    tumblr               = 0xf173,
    tumblrsquare         = 0xf174,
    turkishlira          = 0xf195,
    tv                   = 0xf26c,
    twitch               = 0xf1e8,
    twitter              = 0xf099,
    twittersquare        = 0xf081,
    umbrella             = 0xf0e9,
    underline            = 0xf0cd,
    undo                 = 0xf0e2,
    university           = 0xf19c,
    unlink               = 0xf127,
    unlock               = 0xf09c,
    unlockalt            = 0xf13e,
    unsorted             = 0xf0dc,
    upload               = 0xf093,
    usd                  = 0xf155,
    user                 = 0xf007,
    usermd               = 0xf0f0,
    userplus             = 0xf234,
    usersecret           = 0xf21b,
    usertimes            = 0xf235,
    users                = 0xf0c0,
    venus                = 0xf221,
    venusdouble          = 0xf226,
    venusmars            = 0xf228,
    viacoin              = 0xf237,
    videocamera          = 0xf03d,
    vimeo                = 0xf27d,
    vimeosquare          = 0xf194,
    vine                 = 0xf1ca,
    vk                   = 0xf189,
    volumedown           = 0xf027,
    volumeoff            = 0xf026,
    volumeup             = 0xf028,
    warning              = 0xf071,
    wechat               = 0xf1d7,
    weibo                = 0xf18a,
    weixin               = 0xf1d7,
    whatsapp             = 0xf232,
    wheelchair           = 0xf193,
    wifi                 = 0xf1eb,
    wikipediaw           = 0xf266,
    windows              = 0xf17a,
    won                  = 0xf159,
    wordpress            = 0xf19a,
    wrench               = 0xf0ad,
    xing                 = 0xf168,
    xingsquare           = 0xf169,
    ycombinator          = 0xf23b,
    ycombinatorsquare    = 0xf1d4,
    yahoo                = 0xf19e,
    yc                   = 0xf23b,
    ycsquare             = 0xf1d4,
    yelp                 = 0xf1e9,
    yen                  = 0xf157,
    youtube              = 0xf167,
    youtubeplay          = 0xf16a,
    youtubesquare        = 0xf166,

    gitlab = 0xf2a3,
    wpbeginner = 0xf2a3,
    wpforms = 0xf2a2,
    envira = 0xf29e,
    universalaccess = 0xf29d,
    wheelchairalt = 0xf2a1,
    questioncircleo = 0xf2a4,
    blind = 0xf2a4,
    audiodescription = 0xf299,
    volumecontrolphone = 0xf2b4,
    braille = 0xf2b0,
    assistivelisteningsystems = 0xf2b4,
    aslinterpreting = 0xf296,
    americansignlanguageinterpreting = 0xf2a5,
    deafness = 0xf2a6,
    hardofhearing = 0xf2b3,
    deaf = 0xf2b3,
    glide = 0xf2a4,
    glideg = 0xf2a8,
    signing = 0xf2ae,
    signlanguage = 0xf29c,
    lowvision = 0xf2a7,
    viadeo = 0xf2a7,
    viadeosquare = 0xf2ab,
    snapchat = 0xf2ac,
    snapchatghost = 0xf2ad,
    snapchatsquare = 0xf2b2,
    firstorder = 0xf2a9,
    yoast = 0xf2aa,
    themeisle = 0xf2a0,
    googlepluscircle = 0xf29b,
    googleplusofficial = 0xf297,
    fa_ = 0xf298,
    fontawesome = 0xf2b1
  };
};

}

#endif // TULIPFONTAWESOME_H

