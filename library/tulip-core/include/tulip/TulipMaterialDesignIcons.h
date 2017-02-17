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

#ifndef TULIPMATERIALDESIGNICONS_H
#define TULIPMATERIALDESIGNICONS_H

#include <tulip/tulipconf.h>

#include <vector>

#undef linux

namespace tlp {

/**
 * @brief Helper class for the configuration of a Material Design Icon glyph.
 *
 * Material Design Icons (https://materialdesignicons.com/) : a growing icon collection allows designers and developers
 * targeting various platforms to download icons in the format, color and size they need for any project.
 *
 * A glyph has been added to Tulip enabling to use these great icons
 * as nodes and edges extremities shapes.
 *
 * That class offers utility functions and supported icons names constants.
 *
 * To set a node glyph as a Material Design icon, the Material Design Icon glyph must be associated
 * to the node through the modification of the "viewShape" integer property attached to the graph.
 * The name of the icon to use must then be set in the "viewMaterialDesignIcon" string property.
 * As an example, the following code snippet activates the Material Design glyph for all nodes
 * and sets the "user" icon.
 *
 * @code
 *    // graph is a pointer to a tlp::Graph object
 *    tlp::IntegerProperty *viewShape = graph->getProperty<tlp::IntegerProperty>("viewShape");
 *    tlp::StringProperty *viewMaterialDesignIcon = graph->getProperty<tlp::StringProperty>("viewMaterialDesignIcon");
 *
 *    // sets the Material Design glyph on all nodes
 *    viewShape->setAllNodeValue(tlp::NodeShape::MaterialDesignIcon);
 *    // sets the "user" glyph for all nodes
 *    viewMaterialDesignIconsIcon->setAllNodeValue(tlp::TulipMaterialDesignIcons::User);
 * @endcode
 **/

class TLP_SCOPE TulipMaterialDesignIcons {

public:
  /**
   * Returns the location of the Material Design Icons TrueType font file bundled with Tulip
   */
  static std::string getMaterialDesignIconsTrueTypeFileLocation();

  /**
   * Returns the list of supported Material Design icons names
   */
  static const std::vector<std::string> &getSupportedMaterialDesignIcons();

  /**
   * Checks if the provided Material Design icon name is supported
   * @param iconName the name of the icon to check support
   */
  static bool isMaterialDesignIconSupported(const std::string &iconName);

  /**
   * Returns the Unicode code point associated to an icon name
   * @param iconName the name of the icon to get the codepoint
   */
  static unsigned int getMaterialDesignIconCodePoint(const std::string &iconName);

  /**
   * Returns an UTF-8 encoded string of a Material Design icon
   * @param iconName a Material Design icon name
   * @return
   */
  static std::string getMaterialDesignIconUtf8String(const std::string &iconName);

  static const char *AccessPoint;
  static const char *AccessPointNetwork;
  static const char *Account;
  static const char *AccountAlert;
  static const char *AccountBox;
  static const char *AccountBoxOutline;
  static const char *AccountCardDetails;
  static const char *AccountCheck;
  static const char *AccountCircle;
  static const char *AccountConvert;
  static const char *AccountEdit;
  static const char *AccountKey;
  static const char *AccountLocation;
  static const char *AccountMinus;
  static const char *AccountMultiple;
  static const char *AccountMultipleMinus;
  static const char *AccountMultipleOutline;
  static const char *AccountMultiplePlus;
  static const char *AccountNetwork;
  static const char *AccountOff;
  static const char *AccountOutline;
  static const char *AccountPlus;
  static const char *AccountRemove;
  static const char *AccountSearch;
  static const char *AccountSettings;
  static const char *AccountSettingsVariant;
  static const char *AccountStar;
  static const char *AccountStarVariant;
  static const char *AccountSwitch;
  static const char *Adjust;
  static const char *AirConditioner;
  static const char *Airballoon;
  static const char *Airplane;
  static const char *AirplaneLanding;
  static const char *AirplaneOff;
  static const char *AirplaneTakeoff;
  static const char *Airplay;
  static const char *Alarm;
  static const char *AlarmCheck;
  static const char *AlarmMultiple;
  static const char *AlarmOff;
  static const char *AlarmPlus;
  static const char *AlarmSnooze;
  static const char *Album;
  static const char *Alert;
  static const char *AlertBox;
  static const char *AlertCircle;
  static const char *AlertCircleOutline;
  static const char *AlertOctagon;
  static const char *AlertOctagram;
  static const char *AlertOutline;
  static const char *AllInclusive;
  static const char *Alpha;
  static const char *Alphabetical;
  static const char *Altimeter;
  static const char *Amazon;
  static const char *AmazonClouddrive;
  static const char *Ambulance;
  static const char *Amplifier;
  static const char *Anchor;
  static const char *Android;
  static const char *AndroidDebugBridge;
  static const char *AndroidStudio;
  static const char *Angular;
  static const char *Angularjs;
  static const char *Animation;
  static const char *Apple;
  static const char *AppleFinder;
  static const char *AppleIos;
  static const char *AppleKeyboardCaps;
  static const char *AppleKeyboardCommand;
  static const char *AppleKeyboardControl;
  static const char *AppleKeyboardOption;
  static const char *AppleKeyboardShift;
  static const char *AppleMobileme;
  static const char *AppleSafari;
  static const char *Application;
  static const char *Apps;
  static const char *Archive;
  static const char *ArrangeBringForward;
  static const char *ArrangeBringToFront;
  static const char *ArrangeSendBackward;
  static const char *ArrangeSendToBack;
  static const char *ArrowAll;
  static const char *ArrowBottomLeft;
  static const char *ArrowBottomRight;
  static const char *ArrowCompress;
  static const char *ArrowCompressAll;
  static const char *ArrowDown;
  static const char *ArrowDownBold;
  static const char *ArrowDownBoldCircle;
  static const char *ArrowDownBoldCircleOutline;
  static const char *ArrowDownBoldHexagonOutline;
  static const char *ArrowDownBox;
  static const char *ArrowDownDropCircle;
  static const char *ArrowDownDropCircleOutline;
  static const char *ArrowExpand;
  static const char *ArrowExpandAll;
  static const char *ArrowLeft;
  static const char *ArrowLeftBold;
  static const char *ArrowLeftBoldCircle;
  static const char *ArrowLeftBoldCircleOutline;
  static const char *ArrowLeftBoldHexagonOutline;
  static const char *ArrowLeftBox;
  static const char *ArrowLeftDropCircle;
  static const char *ArrowLeftDropCircleOutline;
  static const char *ArrowRight;
  static const char *ArrowRightBold;
  static const char *ArrowRightBoldCircle;
  static const char *ArrowRightBoldCircleOutline;
  static const char *ArrowRightBoldHexagonOutline;
  static const char *ArrowRightBox;
  static const char *ArrowRightDropCircle;
  static const char *ArrowRightDropCircleOutline;
  static const char *ArrowTopLeft;
  static const char *ArrowTopRight;
  static const char *ArrowUp;
  static const char *ArrowUpBold;
  static const char *ArrowUpBoldCircle;
  static const char *ArrowUpBoldCircleOutline;
  static const char *ArrowUpBoldHexagonOutline;
  static const char *ArrowUpBox;
  static const char *ArrowUpDropCircle;
  static const char *ArrowUpDropCircleOutline;
  static const char *Assistant;
  static const char *Asterisk;
  static const char *At;
  static const char *Attachment;
  static const char *Audiobook;
  static const char *AutoFix;
  static const char *AutoUpload;
  static const char *Autorenew;
  static const char *AvTimer;
  static const char *Baby;
  static const char *BabyBuggy;
  static const char *Backburger;
  static const char *Backspace;
  static const char *BackupRestore;
  static const char *Bandcamp;
  static const char *Bank;
  static const char *Barcode;
  static const char *BarcodeScan;
  static const char *Barley;
  static const char *Barrel;
  static const char *Basecamp;
  static const char *Basket;
  static const char *BasketFill;
  static const char *BasketUnfill;
  static const char *Battery;
  static const char *Battery10;
  static const char *Battery20;
  static const char *Battery30;
  static const char *Battery40;
  static const char *Battery50;
  static const char *Battery60;
  static const char *Battery70;
  static const char *Battery80;
  static const char *Battery90;
  static const char *BatteryAlert;
  static const char *BatteryCharging;
  static const char *BatteryCharging100;
  static const char *BatteryCharging20;
  static const char *BatteryCharging30;
  static const char *BatteryCharging40;
  static const char *BatteryCharging60;
  static const char *BatteryCharging80;
  static const char *BatteryCharging90;
  static const char *BatteryMinus;
  static const char *BatteryNegative;
  static const char *BatteryOutline;
  static const char *BatteryPlus;
  static const char *BatteryPositive;
  static const char *BatteryUnknown;
  static const char *Beach;
  static const char *Beaker;
  static const char *Beats;
  static const char *Beer;
  static const char *Behance;
  static const char *Bell;
  static const char *BellOff;
  static const char *BellOutline;
  static const char *BellPlus;
  static const char *BellRing;
  static const char *BellRingOutline;
  static const char *BellSleep;
  static const char *Beta;
  static const char *Bible;
  static const char *Bike;
  static const char *Bing;
  static const char *Binoculars;
  static const char *Bio;
  static const char *Biohazard;
  static const char *Bitbucket;
  static const char *BlackMesa;
  static const char *Blackberry;
  static const char *Blender;
  static const char *Blinds;
  static const char *BlockHelper;
  static const char *Blogger;
  static const char *Bluetooth;
  static const char *BluetoothAudio;
  static const char *BluetoothConnect;
  static const char *BluetoothOff;
  static const char *BluetoothSettings;
  static const char *BluetoothTransfer;
  static const char *Blur;
  static const char *BlurLinear;
  static const char *BlurOff;
  static const char *BlurRadial;
  static const char *Bomb;
  static const char *BombOff;
  static const char *Bone;
  static const char *Book;
  static const char *BookMinus;
  static const char *BookMultiple;
  static const char *BookMultipleVariant;
  static const char *BookOpen;
  static const char *BookOpenPageVariant;
  static const char *BookOpenVariant;
  static const char *BookPlus;
  static const char *BookVariant;
  static const char *Bookmark;
  static const char *BookmarkCheck;
  static const char *BookmarkMusic;
  static const char *BookmarkOutline;
  static const char *BookmarkPlus;
  static const char *BookmarkPlusOutline;
  static const char *BookmarkRemove;
  static const char *Boombox;
  static const char *Bootstrap;
  static const char *BorderAll;
  static const char *BorderBottom;
  static const char *BorderColor;
  static const char *BorderHorizontal;
  static const char *BorderInside;
  static const char *BorderLeft;
  static const char *BorderNone;
  static const char *BorderOutside;
  static const char *BorderRight;
  static const char *BorderStyle;
  static const char *BorderTop;
  static const char *BorderVertical;
  static const char *BowTie;
  static const char *Bowl;
  static const char *Bowling;
  static const char *Box;
  static const char *BoxCutter;
  static const char *BoxShadow;
  static const char *Bridge;
  static const char *Briefcase;
  static const char *BriefcaseCheck;
  static const char *BriefcaseDownload;
  static const char *BriefcaseUpload;
  static const char *Brightness1;
  static const char *Brightness2;
  static const char *Brightness3;
  static const char *Brightness4;
  static const char *Brightness5;
  static const char *Brightness6;
  static const char *Brightness7;
  static const char *BrightnessAuto;
  static const char *Broom;
  static const char *Brush;
  static const char *Buffer;
  static const char *Bug;
  static const char *BulletinBoard;
  static const char *Bullhorn;
  static const char *Bullseye;
  static const char *BurstMode;
  static const char *Bus;
  static const char *Cached;
  static const char *Cake;
  static const char *CakeLayered;
  static const char *CakeVariant;
  static const char *Calculator;
  static const char *Calendar;
  static const char *CalendarBlank;
  static const char *CalendarCheck;
  static const char *CalendarClock;
  static const char *CalendarMultiple;
  static const char *CalendarMultipleCheck;
  static const char *CalendarPlus;
  static const char *CalendarQuestion;
  static const char *CalendarRange;
  static const char *CalendarRemove;
  static const char *CalendarText;
  static const char *CalendarToday;
  static const char *CallMade;
  static const char *CallMerge;
  static const char *CallMissed;
  static const char *CallReceived;
  static const char *CallSplit;
  static const char *Camcorder;
  static const char *CamcorderBox;
  static const char *CamcorderBoxOff;
  static const char *CamcorderOff;
  static const char *Camera;
  static const char *CameraBurst;
  static const char *CameraEnhance;
  static const char *CameraFront;
  static const char *CameraFrontVariant;
  static const char *CameraIris;
  static const char *CameraOff;
  static const char *CameraPartyMode;
  static const char *CameraRear;
  static const char *CameraRearVariant;
  static const char *CameraSwitch;
  static const char *CameraTimer;
  static const char *Candle;
  static const char *Candycane;
  static const char *Car;
  static const char *CarBattery;
  static const char *CarConnected;
  static const char *CarWash;
  static const char *Cards;
  static const char *CardsOutline;
  static const char *CardsPlayingOutline;
  static const char *CardsVariant;
  static const char *Carrot;
  static const char *Cart;
  static const char *CartOff;
  static const char *CartOutline;
  static const char *CartPlus;
  static const char *CaseSensitiveAlt;
  static const char *Cash;
  static const char *Cash100;
  static const char *CashMultiple;
  static const char *CashUsd;
  static const char *Cast;
  static const char *CastConnected;
  static const char *Castle;
  static const char *Cat;
  static const char *Cellphone;
  static const char *CellphoneAndroid;
  static const char *CellphoneBasic;
  static const char *CellphoneDock;
  static const char *CellphoneIphone;
  static const char *CellphoneLink;
  static const char *CellphoneLinkOff;
  static const char *CellphoneSettings;
  static const char *Certificate;
  static const char *ChairSchool;
  static const char *ChartArc;
  static const char *ChartAreaspline;
  static const char *ChartBar;
  static const char *ChartBubble;
  static const char *ChartGantt;
  static const char *ChartHistogram;
  static const char *ChartLine;
  static const char *ChartPie;
  static const char *ChartScatterplotHexbin;
  static const char *ChartTimeline;
  static const char *Check;
  static const char *CheckAll;
  static const char *CheckCircle;
  static const char *CheckCircleOutline;
  static const char *CheckboxBlank;
  static const char *CheckboxBlankCircle;
  static const char *CheckboxBlankCircleOutline;
  static const char *CheckboxBlankOutline;
  static const char *CheckboxMarked;
  static const char *CheckboxMarkedCircle;
  static const char *CheckboxMarkedCircleOutline;
  static const char *CheckboxMarkedOutline;
  static const char *CheckboxMultipleBlank;
  static const char *CheckboxMultipleBlankCircle;
  static const char *CheckboxMultipleBlankCircleOutline;
  static const char *CheckboxMultipleBlankOutline;
  static const char *CheckboxMultipleMarked;
  static const char *CheckboxMultipleMarkedCircle;
  static const char *CheckboxMultipleMarkedCircleOutline;
  static const char *CheckboxMultipleMarkedOutline;
  static const char *Checkerboard;
  static const char *ChemicalWeapon;
  static const char *ChevronDoubleDown;
  static const char *ChevronDoubleLeft;
  static const char *ChevronDoubleRight;
  static const char *ChevronDoubleUp;
  static const char *ChevronDown;
  static const char *ChevronLeft;
  static const char *ChevronRight;
  static const char *ChevronUp;
  static const char *Chip;
  static const char *Church;
  static const char *CiscoWebex;
  static const char *City;
  static const char *Clipboard;
  static const char *ClipboardAccount;
  static const char *ClipboardAlert;
  static const char *ClipboardArrowDown;
  static const char *ClipboardArrowLeft;
  static const char *ClipboardCheck;
  static const char *ClipboardFlow;
  static const char *ClipboardOutline;
  static const char *ClipboardText;
  static const char *Clippy;
  static const char *Clock;
  static const char *ClockAlert;
  static const char *ClockEnd;
  static const char *ClockFast;
  static const char *ClockIn;
  static const char *ClockOut;
  static const char *ClockStart;
  static const char *Close;
  static const char *CloseBox;
  static const char *CloseBoxOutline;
  static const char *CloseCircle;
  static const char *CloseCircleOutline;
  static const char *CloseNetwork;
  static const char *CloseOctagon;
  static const char *CloseOctagonOutline;
  static const char *CloseOutline;
  static const char *ClosedCaption;
  static const char *Cloud;
  static const char *CloudCheck;
  static const char *CloudCircle;
  static const char *CloudDownload;
  static const char *CloudOutline;
  static const char *CloudOutlineOff;
  static const char *CloudPrint;
  static const char *CloudPrintOutline;
  static const char *CloudSync;
  static const char *CloudUpload;
  static const char *CodeArray;
  static const char *CodeBraces;
  static const char *CodeBrackets;
  static const char *CodeEqual;
  static const char *CodeGreaterThan;
  static const char *CodeGreaterThanOrEqual;
  static const char *CodeLessThan;
  static const char *CodeLessThanOrEqual;
  static const char *CodeNotEqual;
  static const char *CodeNotEqualVariant;
  static const char *CodeParentheses;
  static const char *CodeString;
  static const char *CodeTags;
  static const char *CodeTagsCheck;
  static const char *Codepen;
  static const char *Coffee;
  static const char *CoffeeOutline;
  static const char *CoffeeToGo;
  static const char *Coin;
  static const char *Coins;
  static const char *Collage;
  static const char *ColorHelper;
  static const char *Comment;
  static const char *CommentAccount;
  static const char *CommentAccountOutline;
  static const char *CommentAlert;
  static const char *CommentAlertOutline;
  static const char *CommentCheck;
  static const char *CommentCheckOutline;
  static const char *CommentMultipleOutline;
  static const char *CommentOutline;
  static const char *CommentPlusOutline;
  static const char *CommentProcessing;
  static const char *CommentProcessingOutline;
  static const char *CommentQuestionOutline;
  static const char *CommentRemoveOutline;
  static const char *CommentText;
  static const char *CommentTextOutline;
  static const char *Compare;
  static const char *Compass;
  static const char *CompassOutline;
  static const char *Console;
  static const char *ContactMail;
  static const char *Contacts;
  static const char *ContentCopy;
  static const char *ContentCut;
  static const char *ContentDuplicate;
  static const char *ContentPaste;
  static const char *ContentSave;
  static const char *ContentSaveAll;
  static const char *ContentSaveSettings;
  static const char *Contrast;
  static const char *ContrastBox;
  static const char *ContrastCircle;
  static const char *Cookie;
  static const char *Copyright;
  static const char *Counter;
  static const char *Cow;
  static const char *Creation;
  static const char *CreditCard;
  static const char *CreditCardMultiple;
  static const char *CreditCardOff;
  static const char *CreditCardPlus;
  static const char *CreditCardScan;
  static const char *Crop;
  static const char *CropFree;
  static const char *CropLandscape;
  static const char *CropPortrait;
  static const char *CropRotate;
  static const char *CropSquare;
  static const char *Crosshairs;
  static const char *CrosshairsGps;
  static const char *Crown;
  static const char *Cube;
  static const char *CubeOutline;
  static const char *CubeSend;
  static const char *CubeUnfolded;
  static const char *Cup;
  static const char *CupOff;
  static const char *CupWater;
  static const char *CurrencyBtc;
  static const char *CurrencyEur;
  static const char *CurrencyGbp;
  static const char *CurrencyInr;
  static const char *CurrencyNgn;
  static const char *CurrencyRub;
  static const char *CurrencyTry;
  static const char *CurrencyUsd;
  static const char *CurrencyUsdOff;
  static const char *CursorDefault;
  static const char *CursorDefaultOutline;
  static const char *CursorMove;
  static const char *CursorPointer;
  static const char *CursorText;
  static const char *Database;
  static const char *DatabaseMinus;
  static const char *DatabasePlus;
  static const char *DebugStepInto;
  static const char *DebugStepOut;
  static const char *DebugStepOver;
  static const char *DecimalDecrease;
  static const char *DecimalIncrease;
  static const char *Delete;
  static const char *DeleteCircle;
  static const char *DeleteEmpty;
  static const char *DeleteForever;
  static const char *DeleteSweep;
  static const char *DeleteVariant;
  static const char *Delta;
  static const char *Deskphone;
  static const char *DesktopMac;
  static const char *DesktopTower;
  static const char *Details;
  static const char *DeveloperBoard;
  static const char *Deviantart;
  static const char *Dialpad;
  static const char *Diamond;
  static const char *Dice1;
  static const char *Dice2;
  static const char *Dice3;
  static const char *Dice4;
  static const char *Dice5;
  static const char *Dice6;
  static const char *DiceD20;
  static const char *DiceD4;
  static const char *DiceD6;
  static const char *DiceD8;
  static const char *Dictionary;
  static const char *Directions;
  static const char *DirectionsFork;
  static const char *Discord;
  static const char *Disk;
  static const char *DiskAlert;
  static const char *Disqus;
  static const char *DisqusOutline;
  static const char *Division;
  static const char *DivisionBox;
  static const char *Dna;
  static const char *Dns;
  static const char *DoNotDisturb;
  static const char *DoNotDisturbOff;
  static const char *Dolby;
  static const char *Domain;
  static const char *DotsHorizontal;
  static const char *DotsVertical;
  static const char *Douban;
  static const char *Download;
  static const char *Drag;
  static const char *DragHorizontal;
  static const char *DragVertical;
  static const char *Drawing;
  static const char *DrawingBox;
  static const char *Dribbble;
  static const char *DribbbleBox;
  static const char *Drone;
  static const char *Dropbox;
  static const char *Drupal;
  static const char *Duck;
  static const char *Dumbbell;
  static const char *Earth;
  static const char *EarthBox;
  static const char *EarthBoxOff;
  static const char *EarthOff;
  static const char *Edge;
  static const char *Eject;
  static const char *ElevationDecline;
  static const char *ElevationRise;
  static const char *Elevator;
  static const char *Email;
  static const char *EmailAlert;
  static const char *EmailOpen;
  static const char *EmailOpenOutline;
  static const char *EmailOutline;
  static const char *EmailSecure;
  static const char *EmailVariant;
  static const char *Emby;
  static const char *Emoticon;
  static const char *EmoticonCool;
  static const char *EmoticonDead;
  static const char *EmoticonDevil;
  static const char *EmoticonExcited;
  static const char *EmoticonHappy;
  static const char *EmoticonNeutral;
  static const char *EmoticonPoop;
  static const char *EmoticonSad;
  static const char *EmoticonTongue;
  static const char *Engine;
  static const char *EngineOutline;
  static const char *Equal;
  static const char *EqualBox;
  static const char *Eraser;
  static const char *EraserVariant;
  static const char *Escalator;
  static const char *Ethernet;
  static const char *EthernetCable;
  static const char *EthernetCableOff;
  static const char *Etsy;
  static const char *EvStation;
  static const char *Evernote;
  static const char *Exclamation;
  static const char *ExitToApp;
  static const char *Export;
  static const char *Eye;
  static const char *EyeOff;
  static const char *EyeOutline;
  static const char *EyeOutlineOff;
  static const char *Eyedropper;
  static const char *EyedropperVariant;
  static const char *Face;
  static const char *FaceProfile;
  static const char *Facebook;
  static const char *FacebookBox;
  static const char *FacebookMessenger;
  static const char *Factory;
  static const char *Fan;
  static const char *FastForward;
  static const char *FastForwardOutline;
  static const char *Fax;
  static const char *Feather;
  static const char *Ferry;
  static const char *File;
  static const char *FileChart;
  static const char *FileCheck;
  static const char *FileCloud;
  static const char *FileDelimited;
  static const char *FileDocument;
  static const char *FileDocumentBox;
  static const char *FileExcel;
  static const char *FileExcelBox;
  static const char *FileExport;
  static const char *FileFind;
  static const char *FileHidden;
  static const char *FileImage;
  static const char *FileImport;
  static const char *FileLock;
  static const char *FileMultiple;
  static const char *FileMusic;
  static const char *FileOutline;
  static const char *FilePdf;
  static const char *FilePdfBox;
  static const char *FilePowerpoint;
  static const char *FilePowerpointBox;
  static const char *FilePresentationBox;
  static const char *FileRestore;
  static const char *FileSend;
  static const char *FileTree;
  static const char *FileVideo;
  static const char *FileWord;
  static const char *FileWordBox;
  static const char *FileXml;
  static const char *Film;
  static const char *Filmstrip;
  static const char *FilmstripOff;
  static const char *Filter;
  static const char *FilterOutline;
  static const char *FilterRemove;
  static const char *FilterRemoveOutline;
  static const char *FilterVariant;
  static const char *FindReplace;
  static const char *Fingerprint;
  static const char *Fire;
  static const char *Firefox;
  static const char *Fish;
  static const char *Flag;
  static const char *FlagCheckered;
  static const char *FlagOutline;
  static const char *FlagOutlineVariant;
  static const char *FlagTriangle;
  static const char *FlagVariant;
  static const char *Flash;
  static const char *FlashAuto;
  static const char *FlashOff;
  static const char *FlashOutline;
  static const char *FlashRedEye;
  static const char *Flashlight;
  static const char *FlashlightOff;
  static const char *Flask;
  static const char *FlaskEmpty;
  static const char *FlaskEmptyOutline;
  static const char *FlaskOutline;
  static const char *Flattr;
  static const char *FlipToBack;
  static const char *FlipToFront;
  static const char *Floppy;
  static const char *Flower;
  static const char *Folder;
  static const char *FolderAccount;
  static const char *FolderDownload;
  static const char *FolderGoogleDrive;
  static const char *FolderImage;
  static const char *FolderLock;
  static const char *FolderLockOpen;
  static const char *FolderMove;
  static const char *FolderMultiple;
  static const char *FolderMultipleImage;
  static const char *FolderMultipleOutline;
  static const char *FolderOutline;
  static const char *FolderPlus;
  static const char *FolderRemove;
  static const char *FolderStar;
  static const char *FolderUpload;
  static const char *FontAwesome;
  static const char *Food;
  static const char *FoodApple;
  static const char *FoodForkDrink;
  static const char *FoodOff;
  static const char *FoodVariant;
  static const char *Football;
  static const char *FootballAustralian;
  static const char *FootballHelmet;
  static const char *FormatAlignCenter;
  static const char *FormatAlignJustify;
  static const char *FormatAlignLeft;
  static const char *FormatAlignRight;
  static const char *FormatAnnotationPlus;
  static const char *FormatBold;
  static const char *FormatClear;
  static const char *FormatColorFill;
  static const char *FormatColorText;
  static const char *FormatFloatCenter;
  static const char *FormatFloatLeft;
  static const char *FormatFloatNone;
  static const char *FormatFloatRight;
  static const char *FormatFont;
  static const char *FormatHeader1;
  static const char *FormatHeader2;
  static const char *FormatHeader3;
  static const char *FormatHeader4;
  static const char *FormatHeader5;
  static const char *FormatHeader6;
  static const char *FormatHeaderDecrease;
  static const char *FormatHeaderEqual;
  static const char *FormatHeaderIncrease;
  static const char *FormatHeaderPound;
  static const char *FormatHorizontalAlignCenter;
  static const char *FormatHorizontalAlignLeft;
  static const char *FormatHorizontalAlignRight;
  static const char *FormatIndentDecrease;
  static const char *FormatIndentIncrease;
  static const char *FormatItalic;
  static const char *FormatLineSpacing;
  static const char *FormatLineStyle;
  static const char *FormatLineWeight;
  static const char *FormatListBulleted;
  static const char *FormatListBulletedType;
  static const char *FormatListNumbers;
  static const char *FormatPageBreak;
  static const char *FormatPaint;
  static const char *FormatParagraph;
  static const char *FormatPilcrow;
  static const char *FormatQuote;
  static const char *FormatRotate90;
  static const char *FormatSection;
  static const char *FormatSize;
  static const char *FormatStrikethrough;
  static const char *FormatStrikethroughVariant;
  static const char *FormatSubscript;
  static const char *FormatSuperscript;
  static const char *FormatText;
  static const char *FormatTextdirectionLToR;
  static const char *FormatTextdirectionRToL;
  static const char *FormatTitle;
  static const char *FormatUnderline;
  static const char *FormatVerticalAlignBottom;
  static const char *FormatVerticalAlignCenter;
  static const char *FormatVerticalAlignTop;
  static const char *FormatWrapInline;
  static const char *FormatWrapSquare;
  static const char *FormatWrapTight;
  static const char *FormatWrapTopBottom;
  static const char *Forum;
  static const char *Forward;
  static const char *Foursquare;
  static const char *Fridge;
  static const char *FridgeFilled;
  static const char *FridgeFilledBottom;
  static const char *FridgeFilledTop;
  static const char *Fullscreen;
  static const char *FullscreenExit;
  static const char *Function;
  static const char *Gamepad;
  static const char *GamepadVariant;
  static const char *Garage;
  static const char *GarageOpen;
  static const char *GasCylinder;
  static const char *GasStation;
  static const char *Gate;
  static const char *Gauge;
  static const char *Gavel;
  static const char *GenderFemale;
  static const char *GenderMale;
  static const char *GenderMaleFemale;
  static const char *GenderTransgender;
  static const char *Ghost;
  static const char *Gift;
  static const char *Git;
  static const char *GithubBox;
  static const char *GithubCircle;
  static const char *GithubFace;
  static const char *GlassFlute;
  static const char *GlassMug;
  static const char *GlassStange;
  static const char *GlassTulip;
  static const char *Glassdoor;
  static const char *Glasses;
  static const char *Gmail;
  static const char *Gnome;
  static const char *Gondola;
  static const char *Google;
  static const char *GoogleCardboard;
  static const char *GoogleChrome;
  static const char *GoogleCircles;
  static const char *GoogleCirclesCommunities;
  static const char *GoogleCirclesExtended;
  static const char *GoogleCirclesGroup;
  static const char *GoogleController;
  static const char *GoogleControllerOff;
  static const char *GoogleDrive;
  static const char *GoogleEarth;
  static const char *GoogleGlass;
  static const char *GoogleKeep;
  static const char *GoogleMaps;
  static const char *GoogleNearby;
  static const char *GooglePages;
  static const char *GooglePhotos;
  static const char *GooglePhysicalWeb;
  static const char *GooglePlay;
  static const char *GooglePlus;
  static const char *GooglePlusBox;
  static const char *GoogleTranslate;
  static const char *GoogleWallet;
  static const char *Gradient;
  static const char *GreasePencil;
  static const char *Grid;
  static const char *GridOff;
  static const char *Group;
  static const char *GuitarElectric;
  static const char *GuitarPick;
  static const char *GuitarPickOutline;
  static const char *Hackernews;
  static const char *Hamburger;
  static const char *HandPointingRight;
  static const char *Hanger;
  static const char *Hangouts;
  static const char *Harddisk;
  static const char *Headphones;
  static const char *HeadphonesBox;
  static const char *HeadphonesSettings;
  static const char *Headset;
  static const char *HeadsetDock;
  static const char *HeadsetOff;
  static const char *Heart;
  static const char *HeartBox;
  static const char *HeartBoxOutline;
  static const char *HeartBroken;
  static const char *HeartHalfOutline;
  static const char *HeartHalfPart;
  static const char *HeartHalfPartOutline;
  static const char *HeartOutline;
  static const char *HeartPulse;
  static const char *Help;
  static const char *HelpCircle;
  static const char *HelpCircleOutline;
  static const char *Hexagon;
  static const char *HexagonMultiple;
  static const char *HexagonOutline;
  static const char *Highway;
  static const char *History;
  static const char *Hololens;
  static const char *Home;
  static const char *HomeMapMarker;
  static const char *HomeModern;
  static const char *HomeOutline;
  static const char *HomeVariant;
  static const char *Hook;
  static const char *HookOff;
  static const char *Hops;
  static const char *Hospital;
  static const char *HospitalBuilding;
  static const char *HospitalMarker;
  static const char *Hotel;
  static const char *Houzz;
  static const char *HouzzBox;
  static const char *Human;
  static const char *HumanChild;
  static const char *HumanFemale;
  static const char *HumanGreeting;
  static const char *HumanHandsdown;
  static const char *HumanHandsup;
  static const char *HumanMale;
  static const char *HumanMaleFemale;
  static const char *HumanPregnant;
  static const char *Image;
  static const char *ImageAlbum;
  static const char *ImageArea;
  static const char *ImageAreaClose;
  static const char *ImageBroken;
  static const char *ImageBrokenVariant;
  static const char *ImageFilter;
  static const char *ImageFilterBlackWhite;
  static const char *ImageFilterCenterFocus;
  static const char *ImageFilterCenterFocusWeak;
  static const char *ImageFilterDrama;
  static const char *ImageFilterFrames;
  static const char *ImageFilterHdr;
  static const char *ImageFilterNone;
  static const char *ImageFilterTiltShift;
  static const char *ImageFilterVintage;
  static const char *ImageMultiple;
  static const char *Import;
  static const char *Inbox;
  static const char *InboxArrowDown;
  static const char *InboxArrowUp;
  static const char *Incognito;
  static const char *Infinity;
  static const char *Information;
  static const char *InformationOutline;
  static const char *InformationVariant;
  static const char *Instagram;
  static const char *Instapaper;
  static const char *InternetExplorer;
  static const char *InvertColors;
  static const char *Itunes;
  static const char *Jeepney;
  static const char *Jira;
  static const char *Jsfiddle;
  static const char *Json;
  static const char *Keg;
  static const char *Kettle;
  static const char *Key;
  static const char *KeyChange;
  static const char *KeyMinus;
  static const char *KeyPlus;
  static const char *KeyRemove;
  static const char *KeyVariant;
  static const char *Keyboard;
  static const char *KeyboardBackspace;
  static const char *KeyboardCaps;
  static const char *KeyboardClose;
  static const char *KeyboardOff;
  static const char *KeyboardReturn;
  static const char *KeyboardTab;
  static const char *KeyboardVariant;
  static const char *Kodi;
  static const char *Label;
  static const char *LabelOutline;
  static const char *Lambda;
  static const char *Lamp;
  static const char *Lan;
  static const char *LanConnect;
  static const char *LanDisconnect;
  static const char *LanPending;
  static const char *LanguageC;
  static const char *LanguageCpp;
  static const char *LanguageCsharp;
  static const char *LanguageCss3;
  static const char *LanguageHtml5;
  static const char *LanguageJavascript;
  static const char *LanguagePhp;
  static const char *LanguagePython;
  static const char *LanguagePythonText;
  static const char *LanguageSwift;
  static const char *LanguageTypescript;
  static const char *Laptop;
  static const char *LaptopChromebook;
  static const char *LaptopMac;
  static const char *LaptopOff;
  static const char *LaptopWindows;
  static const char *Lastfm;
  static const char *Launch;
  static const char *Layers;
  static const char *LayersOff;
  static const char *LeadPencil;
  static const char *Leaf;
  static const char *LedOff;
  static const char *LedOn;
  static const char *LedOutline;
  static const char *LedVariantOff;
  static const char *LedVariantOn;
  static const char *LedVariantOutline;
  static const char *Library;
  static const char *LibraryBooks;
  static const char *LibraryMusic;
  static const char *LibraryPlus;
  static const char *Lightbulb;
  static const char *LightbulbOn;
  static const char *LightbulbOnOutline;
  static const char *LightbulbOutline;
  static const char *Link;
  static const char *LinkOff;
  static const char *LinkVariant;
  static const char *LinkVariantOff;
  static const char *Linkedin;
  static const char *LinkedinBox;
  static const char *Linux;
  static const char *Lock;
  static const char *LockOpen;
  static const char *LockOpenOutline;
  static const char *LockOutline;
  static const char *LockPattern;
  static const char *LockPlus;
  static const char *Login;
  static const char *LoginVariant;
  static const char *Logout;
  static const char *LogoutVariant;
  static const char *Looks;
  static const char *Loop;
  static const char *Loupe;
  static const char *Lumx;
  static const char *Magnet;
  static const char *MagnetOn;
  static const char *Magnify;
  static const char *MagnifyMinus;
  static const char *MagnifyMinusOutline;
  static const char *MagnifyPlus;
  static const char *MagnifyPlusOutline;
  static const char *MailRu;
  static const char *Mailbox;
  static const char *Map;
  static const char *MapMarker;
  static const char *MapMarkerCircle;
  static const char *MapMarkerMinus;
  static const char *MapMarkerMultiple;
  static const char *MapMarkerOff;
  static const char *MapMarkerPlus;
  static const char *MapMarkerRadius;
  static const char *Margin;
  static const char *Markdown;
  static const char *Marker;
  static const char *MarkerCheck;
  static const char *Martini;
  static const char *MaterialUi;
  static const char *MathCompass;
  static const char *Matrix;
  static const char *Maxcdn;
  static const char *MedicalBag;
  static const char *Medium;
  static const char *Memory;
  static const char *Menu;
  static const char *MenuDown;
  static const char *MenuDownOutline;
  static const char *MenuLeft;
  static const char *MenuRight;
  static const char *MenuUp;
  static const char *MenuUpOutline;
  static const char *Message;
  static const char *MessageAlert;
  static const char *MessageBulleted;
  static const char *MessageBulletedOff;
  static const char *MessageDraw;
  static const char *MessageImage;
  static const char *MessageOutline;
  static const char *MessagePlus;
  static const char *MessageProcessing;
  static const char *MessageReply;
  static const char *MessageReplyText;
  static const char *MessageSettings;
  static const char *MessageSettingsVariant;
  static const char *MessageText;
  static const char *MessageTextOutline;
  static const char *MessageVideo;
  static const char *Meteor;
  static const char *Microphone;
  static const char *MicrophoneOff;
  static const char *MicrophoneOutline;
  static const char *MicrophoneSettings;
  static const char *MicrophoneVariant;
  static const char *MicrophoneVariantOff;
  static const char *Microscope;
  static const char *Microsoft;
  static const char *Minecraft;
  static const char *Minus;
  static const char *MinusBox;
  static const char *MinusBoxOutline;
  static const char *MinusCircle;
  static const char *MinusCircleOutline;
  static const char *MinusNetwork;
  static const char *Mixcloud;
  static const char *Monitor;
  static const char *MonitorMultiple;
  static const char *More;
  static const char *Motorbike;
  static const char *Mouse;
  static const char *MouseOff;
  static const char *MouseVariant;
  static const char *MouseVariantOff;
  static const char *MoveResize;
  static const char *MoveResizeVariant;
  static const char *Movie;
  static const char *Multiplication;
  static const char *MultiplicationBox;
  static const char *MusicBox;
  static const char *MusicBoxOutline;
  static const char *MusicCircle;
  static const char *MusicNote;
  static const char *MusicNoteBluetooth;
  static const char *MusicNoteBluetoothOff;
  static const char *MusicNoteEighth;
  static const char *MusicNoteHalf;
  static const char *MusicNoteOff;
  static const char *MusicNoteQuarter;
  static const char *MusicNoteSixteenth;
  static const char *MusicNoteWhole;
  static const char *Nature;
  static const char *NaturePeople;
  static const char *Navigation;
  static const char *NearMe;
  static const char *Needle;
  static const char *NestProtect;
  static const char *NestThermostat;
  static const char *Network;
  static const char *NetworkDownload;
  static const char *NetworkQuestion;
  static const char *NetworkUpload;
  static const char *NewBox;
  static const char *Newspaper;
  static const char *Nfc;
  static const char *NfcTap;
  static const char *NfcVariant;
  static const char *Nodejs;
  static const char *Note;
  static const char *NoteMultiple;
  static const char *NoteMultipleOutline;
  static const char *NoteOutline;
  static const char *NotePlus;
  static const char *NotePlusOutline;
  static const char *NoteText;
  static const char *NotificationClearAll;
  static const char *Npm;
  static const char *Nuke;
  static const char *Numeric;
  static const char *Numeric0Box;
  static const char *Numeric0BoxMultipleOutline;
  static const char *Numeric0BoxOutline;
  static const char *Numeric1Box;
  static const char *Numeric1BoxMultipleOutline;
  static const char *Numeric1BoxOutline;
  static const char *Numeric2Box;
  static const char *Numeric2BoxMultipleOutline;
  static const char *Numeric2BoxOutline;
  static const char *Numeric3Box;
  static const char *Numeric3BoxMultipleOutline;
  static const char *Numeric3BoxOutline;
  static const char *Numeric4Box;
  static const char *Numeric4BoxMultipleOutline;
  static const char *Numeric4BoxOutline;
  static const char *Numeric5Box;
  static const char *Numeric5BoxMultipleOutline;
  static const char *Numeric5BoxOutline;
  static const char *Numeric6Box;
  static const char *Numeric6BoxMultipleOutline;
  static const char *Numeric6BoxOutline;
  static const char *Numeric7Box;
  static const char *Numeric7BoxMultipleOutline;
  static const char *Numeric7BoxOutline;
  static const char *Numeric8Box;
  static const char *Numeric8BoxMultipleOutline;
  static const char *Numeric8BoxOutline;
  static const char *Numeric9Box;
  static const char *Numeric9BoxMultipleOutline;
  static const char *Numeric9BoxOutline;
  static const char *Numeric9PlusBox;
  static const char *Numeric9PlusBoxMultipleOutline;
  static const char *Numeric9PlusBoxOutline;
  static const char *Nut;
  static const char *Nutrition;
  static const char *Oar;
  static const char *Octagon;
  static const char *OctagonOutline;
  static const char *Octagram;
  static const char *Odnoklassniki;
  static const char *Office;
  static const char *Oil;
  static const char *OilTemperature;
  static const char *Omega;
  static const char *Onedrive;
  static const char *Opacity;
  static const char *OpenInApp;
  static const char *OpenInNew;
  static const char *Openid;
  static const char *Opera;
  static const char *Ornament;
  static const char *OrnamentVariant;
  static const char *Owl;
  static const char *Package;
  static const char *PackageDown;
  static const char *PackageUp;
  static const char *PackageVariant;
  static const char *PackageVariantClosed;
  static const char *PageFirst;
  static const char *PageLast;
  static const char *PageLayoutBody;
  static const char *PageLayoutFooter;
  static const char *PageLayoutHeader;
  static const char *PageLayoutSidebarLeft;
  static const char *PageLayoutSidebarRight;
  static const char *Palette;
  static const char *PaletteAdvanced;
  static const char *Panda;
  static const char *Pandora;
  static const char *Panorama;
  static const char *PanoramaFisheye;
  static const char *PanoramaHorizontal;
  static const char *PanoramaVertical;
  static const char *PanoramaWideAngle;
  static const char *PaperCutVertical;
  static const char *Paperclip;
  static const char *Parking;
  static const char *Pause;
  static const char *PauseCircle;
  static const char *PauseCircleOutline;
  static const char *PauseOctagon;
  static const char *PauseOctagonOutline;
  static const char *Paw;
  static const char *PawOff;
  static const char *Pen;
  static const char *Pencil;
  static const char *PencilBox;
  static const char *PencilBoxOutline;
  static const char *PencilCircle;
  static const char *PencilLock;
  static const char *PencilOff;
  static const char *Pentagon;
  static const char *PentagonOutline;
  static const char *Percent;
  static const char *Pharmacy;
  static const char *Phone;
  static const char *PhoneBluetooth;
  static const char *PhoneClassic;
  static const char *PhoneForward;
  static const char *PhoneHangup;
  static const char *PhoneInTalk;
  static const char *PhoneIncoming;
  static const char *PhoneLocked;
  static const char *PhoneLog;
  static const char *PhoneMinus;
  static const char *PhoneMissed;
  static const char *PhoneOutgoing;
  static const char *PhonePaused;
  static const char *PhonePlus;
  static const char *PhoneSettings;
  static const char *PhoneVoip;
  static const char *Pi;
  static const char *PiBox;
  static const char *Piano;
  static const char *Pig;
  static const char *Pill;
  static const char *Pillar;
  static const char *Pin;
  static const char *PinOff;
  static const char *PineTree;
  static const char *PineTreeBox;
  static const char *Pinterest;
  static const char *PinterestBox;
  static const char *Pistol;
  static const char *Pizza;
  static const char *PlaneShield;
  static const char *Play;
  static const char *PlayBoxOutline;
  static const char *PlayCircle;
  static const char *PlayCircleOutline;
  static const char *PlayPause;
  static const char *PlayProtectedContent;
  static const char *PlaylistCheck;
  static const char *PlaylistMinus;
  static const char *PlaylistPlay;
  static const char *PlaylistPlus;
  static const char *PlaylistRemove;
  static const char *Playstation;
  static const char *Plex;
  static const char *Plus;
  static const char *PlusBox;
  static const char *PlusBoxOutline;
  static const char *PlusCircle;
  static const char *PlusCircleMultipleOutline;
  static const char *PlusCircleOutline;
  static const char *PlusNetwork;
  static const char *PlusOne;
  static const char *PlusOutline;
  static const char *Pocket;
  static const char *Pokeball;
  static const char *Polaroid;
  static const char *Poll;
  static const char *PollBox;
  static const char *Polymer;
  static const char *Pool;
  static const char *Popcorn;
  static const char *Pot;
  static const char *PotMix;
  static const char *Pound;
  static const char *PoundBox;
  static const char *Power;
  static const char *PowerPlug;
  static const char *PowerPlugOff;
  static const char *PowerSettings;
  static const char *PowerSocket;
  static const char *Prescription;
  static const char *Presentation;
  static const char *PresentationPlay;
  static const char *Printer;
  static const char *Printer3d;
  static const char *PrinterAlert;
  static const char *PrinterSettings;
  static const char *PriorityHigh;
  static const char *PriorityLow;
  static const char *ProfessionalHexagon;
  static const char *Projector;
  static const char *ProjectorScreen;
  static const char *Publish;
  static const char *Pulse;
  static const char *Puzzle;
  static const char *Qqchat;
  static const char *Qrcode;
  static const char *QrcodeScan;
  static const char *Quadcopter;
  static const char *QualityHigh;
  static const char *Quicktime;
  static const char *Radar;
  static const char *Radiator;
  static const char *Radio;
  static const char *RadioHandheld;
  static const char *RadioTower;
  static const char *Radioactive;
  static const char *RadioboxBlank;
  static const char *RadioboxMarked;
  static const char *Raspberrypi;
  static const char *RayEnd;
  static const char *RayEndArrow;
  static const char *RayStart;
  static const char *RayStartArrow;
  static const char *RayStartEnd;
  static const char *RayVertex;
  static const char *Rdio;
  static const char *React;
  static const char *Read;
  static const char *Readability;
  static const char *Receipt;
  static const char *Record;
  static const char *RecordRec;
  static const char *Recycle;
  static const char *Reddit;
  static const char *Redo;
  static const char *RedoVariant;
  static const char *Refresh;
  static const char *Regex;
  static const char *RelativeScale;
  static const char *Reload;
  static const char *Remote;
  static const char *RenameBox;
  static const char *ReorderHorizontal;
  static const char *ReorderVertical;
  static const char *Repeat;
  static const char *RepeatOff;
  static const char *RepeatOnce;
  static const char *Replay;
  static const char *Reply;
  static const char *ReplyAll;
  static const char *Reproduction;
  static const char *ResizeBottomRight;
  static const char *Responsive;
  static const char *Restart;
  static const char *Restore;
  static const char *Rewind;
  static const char *RewindOutline;
  static const char *Rhombus;
  static const char *RhombusOutline;
  static const char *Ribbon;
  static const char *Road;
  static const char *RoadVariant;
  static const char *Robot;
  static const char *Rocket;
  static const char *Roomba;
  static const char *Rotate3d;
  static const char *RotateLeft;
  static const char *RotateLeftVariant;
  static const char *RotateRight;
  static const char *RotateRightVariant;
  static const char *RoundedCorner;
  static const char *RouterWireless;
  static const char *Routes;
  static const char *Rowing;
  static const char *Rss;
  static const char *RssBox;
  static const char *Ruler;
  static const char *Run;
  static const char *RunFast;
  static const char *Sale;
  static const char *Satellite;
  static const char *SatelliteVariant;
  static const char *Saxophone;
  static const char *Scale;
  static const char *ScaleBalance;
  static const char *ScaleBathroom;
  static const char *Scanner;
  static const char *School;
  static const char *ScreenRotation;
  static const char *ScreenRotationLock;
  static const char *Screwdriver;
  static const char *Script;
  static const char *Sd;
  static const char *Seal;
  static const char *SearchWeb;
  static const char *SeatFlat;
  static const char *SeatFlatAngled;
  static const char *SeatIndividualSuite;
  static const char *SeatLegroomExtra;
  static const char *SeatLegroomNormal;
  static const char *SeatLegroomReduced;
  static const char *SeatReclineExtra;
  static const char *SeatReclineNormal;
  static const char *Security;
  static const char *SecurityHome;
  static const char *SecurityNetwork;
  static const char *Select;
  static const char *SelectAll;
  static const char *SelectInverse;
  static const char *SelectOff;
  static const char *Selection;
  static const char *Send;
  static const char *SerialPort;
  static const char *Server;
  static const char *ServerMinus;
  static const char *ServerNetwork;
  static const char *ServerNetworkOff;
  static const char *ServerOff;
  static const char *ServerPlus;
  static const char *ServerRemove;
  static const char *ServerSecurity;
  static const char *Settings;
  static const char *SettingsBox;
  static const char *ShapeCirclePlus;
  static const char *ShapePlus;
  static const char *ShapePolygonPlus;
  static const char *ShapeRectanglePlus;
  static const char *ShapeSquarePlus;
  static const char *Share;
  static const char *ShareVariant;
  static const char *Shield;
  static const char *ShieldOutline;
  static const char *Shopping;
  static const char *ShoppingMusic;
  static const char *Shovel;
  static const char *ShovelOff;
  static const char *Shredder;
  static const char *Shuffle;
  static const char *ShuffleDisabled;
  static const char *ShuffleVariant;
  static const char *Sigma;
  static const char *SigmaLower;
  static const char *SignCaution;
  static const char *Signal;
  static const char *Signal2g;
  static const char *Signal3g;
  static const char *Signal4g;
  static const char *SignalHspa;
  static const char *SignalHspaPlus;
  static const char *SignalVariant;
  static const char *Silverware;
  static const char *SilverwareFork;
  static const char *SilverwareSpoon;
  static const char *SilverwareVariant;
  static const char *Sim;
  static const char *SimAlert;
  static const char *SimOff;
  static const char *Sitemap;
  static const char *SkipBackward;
  static const char *SkipForward;
  static const char *SkipNext;
  static const char *SkipNextCircle;
  static const char *SkipNextCircleOutline;
  static const char *SkipPrevious;
  static const char *SkipPreviousCircle;
  static const char *SkipPreviousCircleOutline;
  static const char *Skull;
  static const char *Skype;
  static const char *SkypeBusiness;
  static const char *Slack;
  static const char *Sleep;
  static const char *SleepOff;
  static const char *Smoking;
  static const char *SmokingOff;
  static const char *Snapchat;
  static const char *Snowflake;
  static const char *Snowman;
  static const char *Soccer;
  static const char *Sofa;
  static const char *Solid;
  static const char *Sort;
  static const char *SortAlphabetical;
  static const char *SortAscending;
  static const char *SortDescending;
  static const char *SortNumeric;
  static const char *SortVariant;
  static const char *Soundcloud;
  static const char *SourceBranch;
  static const char *SourceCommit;
  static const char *SourceCommitEnd;
  static const char *SourceCommitEndLocal;
  static const char *SourceCommitLocal;
  static const char *SourceCommitNextLocal;
  static const char *SourceCommitStart;
  static const char *SourceCommitStartNextLocal;
  static const char *SourceFork;
  static const char *SourceMerge;
  static const char *SourcePull;
  static const char *Speaker;
  static const char *SpeakerOff;
  static const char *SpeakerWireless;
  static const char *Speedometer;
  static const char *Spellcheck;
  static const char *Spotify;
  static const char *Spotlight;
  static const char *SpotlightBeam;
  static const char *Spray;
  static const char *SquareInc;
  static const char *SquareIncCash;
  static const char *Stackexchange;
  static const char *Stackoverflow;
  static const char *Stadium;
  static const char *Stairs;
  static const char *Star;
  static const char *StarCircle;
  static const char *StarHalf;
  static const char *StarOff;
  static const char *StarOutline;
  static const char *Steam;
  static const char *Steering;
  static const char *StepBackward;
  static const char *StepBackward2;
  static const char *StepForward;
  static const char *StepForward2;
  static const char *Stethoscope;
  static const char *Sticker;
  static const char *Stocking;
  static const char *Stop;
  static const char *StopCircle;
  static const char *StopCircleOutline;
  static const char *Store;
  static const char *Store24Hour;
  static const char *Stove;
  static const char *SubdirectoryArrowLeft;
  static const char *SubdirectoryArrowRight;
  static const char *Subway;
  static const char *SubwayVariant;
  static const char *Sunglasses;
  static const char *SurroundSound;
  static const char *Svg;
  static const char *SwapHorizontal;
  static const char *SwapVertical;
  static const char *Swim;
  static const char *Switch;
  static const char *Sword;
  static const char *Sync;
  static const char *SyncAlert;
  static const char *SyncOff;
  static const char *Tab;
  static const char *TabUnselected;
  static const char *Table;
  static const char *TableColumnPlusAfter;
  static const char *TableColumnPlusBefore;
  static const char *TableColumnRemove;
  static const char *TableColumnWidth;
  static const char *TableEdit;
  static const char *TableLarge;
  static const char *TableRowHeight;
  static const char *TableRowPlusAfter;
  static const char *TableRowPlusBefore;
  static const char *TableRowRemove;
  static const char *Tablet;
  static const char *TabletAndroid;
  static const char *TabletIpad;
  static const char *Tag;
  static const char *TagFaces;
  static const char *TagHeart;
  static const char *TagMultiple;
  static const char *TagOutline;
  static const char *TagPlus;
  static const char *TagRemove;
  static const char *TagTextOutline;
  static const char *Target;
  static const char *Taxi;
  static const char *Teamviewer;
  static const char *Telegram;
  static const char *Television;
  static const char *TelevisionGuide;
  static const char *TemperatureCelsius;
  static const char *TemperatureFahrenheit;
  static const char *TemperatureKelvin;
  static const char *Tennis;
  static const char *Tent;
  static const char *Terrain;
  static const char *TestTube;
  static const char *TextShadow;
  static const char *TextToSpeech;
  static const char *TextToSpeechOff;
  static const char *Textbox;
  static const char *Texture;
  static const char *Theater;
  static const char *ThemeLightDark;
  static const char *Thermometer;
  static const char *ThermometerLines;
  static const char *ThumbDown;
  static const char *ThumbDownOutline;
  static const char *ThumbUp;
  static const char *ThumbUpOutline;
  static const char *ThumbsUpDown;
  static const char *Ticket;
  static const char *TicketAccount;
  static const char *TicketConfirmation;
  static const char *TicketPercent;
  static const char *Tie;
  static const char *Tilde;
  static const char *Timelapse;
  static const char *Timer;
  static const char *Timer10;
  static const char *Timer3;
  static const char *TimerOff;
  static const char *TimerSand;
  static const char *TimerSandEmpty;
  static const char *Timetable;
  static const char *ToggleSwitch;
  static const char *ToggleSwitchOff;
  static const char *Tooltip;
  static const char *TooltipEdit;
  static const char *TooltipImage;
  static const char *TooltipOutline;
  static const char *TooltipOutlinePlus;
  static const char *TooltipText;
  static const char *Tooth;
  static const char *Tor;
  static const char *TowerBeach;
  static const char *TowerFire;
  static const char *TrafficLight;
  static const char *Train;
  static const char *Tram;
  static const char *Transcribe;
  static const char *TranscribeClose;
  static const char *Transfer;
  static const char *TransitTransfer;
  static const char *Translate;
  static const char *TreasureChest;
  static const char *Tree;
  static const char *Trello;
  static const char *TrendingDown;
  static const char *TrendingNeutral;
  static const char *TrendingUp;
  static const char *Triangle;
  static const char *TriangleOutline;
  static const char *Trophy;
  static const char *TrophyAward;
  static const char *TrophyOutline;
  static const char *TrophyVariant;
  static const char *TrophyVariantOutline;
  static const char *Truck;
  static const char *TruckDelivery;
  static const char *TruckTrailer;
  static const char *TshirtCrew;
  static const char *TshirtV;
  static const char *Tumblr;
  static const char *TumblrReblog;
  static const char *Tune;
  static const char *TuneVertical;
  static const char *Twitch;
  static const char *Twitter;
  static const char *TwitterBox;
  static const char *TwitterCircle;
  static const char *TwitterRetweet;
  static const char *Ubuntu;
  static const char *Umbraco;
  static const char *Umbrella;
  static const char *UmbrellaOutline;
  static const char *Undo;
  static const char *UndoVariant;
  static const char *UnfoldLess;
  static const char *UnfoldMore;
  static const char *Ungroup;
  static const char *Unity;
  static const char *Untappd;
  static const char *Update;
  static const char *Upload;
  static const char *Usb;
  static const char *VectorArrangeAbove;
  static const char *VectorArrangeBelow;
  static const char *VectorCircle;
  static const char *VectorCircleVariant;
  static const char *VectorCombine;
  static const char *VectorCurve;
  static const char *VectorDifference;
  static const char *VectorDifferenceAb;
  static const char *VectorDifferenceBa;
  static const char *VectorIntersection;
  static const char *VectorLine;
  static const char *VectorPoint;
  static const char *VectorPolygon;
  static const char *VectorPolyline;
  static const char *VectorRectangle;
  static const char *VectorSelection;
  static const char *VectorSquare;
  static const char *VectorTriangle;
  static const char *VectorUnion;
  static const char *Verified;
  static const char *Vibrate;
  static const char *Video;
  static const char *VideoOff;
  static const char *VideoSwitch;
  static const char *ViewAgenda;
  static const char *ViewArray;
  static const char *ViewCarousel;
  static const char *ViewColumn;
  static const char *ViewDashboard;
  static const char *ViewDay;
  static const char *ViewGrid;
  static const char *ViewHeadline;
  static const char *ViewList;
  static const char *ViewModule;
  static const char *ViewParallel;
  static const char *ViewQuilt;
  static const char *ViewSequential;
  static const char *ViewStream;
  static const char *ViewWeek;
  static const char *Vimeo;
  static const char *Vine;
  static const char *Violin;
  static const char *Visualstudio;
  static const char *Vk;
  static const char *VkBox;
  static const char *VkCircle;
  static const char *Vlc;
  static const char *Voice;
  static const char *Voicemail;
  static const char *VolumeHigh;
  static const char *VolumeLow;
  static const char *VolumeMedium;
  static const char *VolumeOff;
  static const char *Vpn;
  static const char *Walk;
  static const char *Wallet;
  static const char *WalletGiftcard;
  static const char *WalletMembership;
  static const char *WalletTravel;
  static const char *Wan;
  static const char *WashingMachine;
  static const char *Watch;
  static const char *WatchExport;
  static const char *WatchImport;
  static const char *WatchVibrate;
  static const char *Water;
  static const char *WaterOff;
  static const char *WaterPercent;
  static const char *WaterPump;
  static const char *Watermark;
  static const char *WeatherCloudy;
  static const char *WeatherFog;
  static const char *WeatherHail;
  static const char *WeatherLightning;
  static const char *WeatherLightningRainy;
  static const char *WeatherNight;
  static const char *WeatherPartlycloudy;
  static const char *WeatherPouring;
  static const char *WeatherRainy;
  static const char *WeatherSnowy;
  static const char *WeatherSnowyRainy;
  static const char *WeatherSunny;
  static const char *WeatherSunset;
  static const char *WeatherSunsetDown;
  static const char *WeatherSunsetUp;
  static const char *WeatherWindy;
  static const char *WeatherWindyVariant;
  static const char *Web;
  static const char *Webcam;
  static const char *Webhook;
  static const char *Webpack;
  static const char *Wechat;
  static const char *Weight;
  static const char *WeightKilogram;
  static const char *Whatsapp;
  static const char *WheelchairAccessibility;
  static const char *WhiteBalanceAuto;
  static const char *WhiteBalanceIncandescent;
  static const char *WhiteBalanceIridescent;
  static const char *WhiteBalanceSunny;
  static const char *Widgets;
  static const char *Wifi;
  static const char *WifiOff;
  static const char *Wii;
  static const char *Wiiu;
  static const char *Wikipedia;
  static const char *WindowClose;
  static const char *WindowClosed;
  static const char *WindowMaximize;
  static const char *WindowMinimize;
  static const char *WindowOpen;
  static const char *WindowRestore;
  static const char *Windows;
  static const char *Wordpress;
  static const char *Worker;
  static const char *Wrap;
  static const char *Wrench;
  static const char *Wunderlist;
  static const char *Xaml;
  static const char *Xbox;
  static const char *XboxController;
  static const char *XboxControllerOff;
  static const char *Xda;
  static const char *Xing;
  static const char *XingBox;
  static const char *XingCircle;
  static const char *Xml;
  static const char *Yeast;
  static const char *Yelp;
  static const char *YinYang;
  static const char *YoutubePlay;
  static const char *ZipBox;

};

class md {

public:
  enum iconCodePoint {
    accesspoint = 0xf002,
    accesspointnetwork = 0xf003,
    account = 0xf004,
    accountalert = 0xf005,
    accountbox = 0xf006,
    accountboxoutline = 0xf007,
    accountcarddetails = 0xf5D2,
    accountcheck = 0xf008,
    accountcircle = 0xf009,
    accountconvert = 0xf00A,
    accountedit = 0xf6BB,
    accountkey = 0xf00B,
    accountlocation = 0xf00C,
    accountminus = 0xf00D,
    accountmultiple = 0xf00E,
    accountmultipleminus = 0xf5D3,
    accountmultipleoutline = 0xf00F,
    accountmultipleplus = 0xf010,
    accountnetwork = 0xf011,
    accountoff = 0xf012,
    accountoutline = 0xf013,
    accountplus = 0xf014,
    accountremove = 0xf015,
    accountsearch = 0xf016,
    accountsettings = 0xf630,
    accountsettingsvariant = 0xf631,
    accountstar = 0xf017,
    accountstarvariant = 0xf018,
    accountswitch = 0xf019,
    adjust = 0xf01A,
    airconditioner = 0xf01B,
    airballoon = 0xf01C,
    airplane = 0xf01D,
    airplanelanding = 0xf5D4,
    airplaneoff = 0xf01E,
    airplanetakeoff = 0xf5D5,
    airplay = 0xf01F,
    alarm = 0xf020,
    alarmcheck = 0xf021,
    alarmmultiple = 0xf022,
    alarmoff = 0xf023,
    alarmplus = 0xf024,
    alarmsnooze = 0xf68D,
    album = 0xf025,
    alert = 0xf026,
    alertbox = 0xf027,
    alertcircle = 0xf028,
    alertcircleoutline = 0xf5D6,
    alertoctagon = 0xf029,
    alertoctagram = 0xf6BC,
    alertoutline = 0xf02A,
    allinclusive = 0xf6BD,
    alpha = 0xf02B,
    alphabetical = 0xf02C,
    altimeter = 0xf5D7,
    amazon = 0xf02D,
    amazonclouddrive = 0xf02E,
    ambulance = 0xf02F,
    amplifier = 0xf030,
    anchor = 0xf031,
    android = 0xf032,
    androiddebugbridge = 0xf033,
    androidstudio = 0xf034,
    angular = 0xf6B1,
    angularjs = 0xf6BE,
    animation = 0xf5D8,
    apple = 0xf035,
    applefinder = 0xf036,
    appleios = 0xf037,
    applekeyboardcaps = 0xf632,
    applekeyboardcommand = 0xf633,
    applekeyboardcontrol = 0xf634,
    applekeyboardoption = 0xf635,
    applekeyboardshift = 0xf636,
    applemobileme = 0xf038,
    applesafari = 0xf039,
    application = 0xf614,
    apps = 0xf03B,
    archive = 0xf03C,
    arrangebringforward = 0xf03D,
    arrangebringtofront = 0xf03E,
    arrangesendbackward = 0xf03F,
    arrangesendtoback = 0xf040,
    arrowall = 0xf041,
    arrowbottomleft = 0xf042,
    arrowbottomright = 0xf043,
    arrowcompress = 0xf615,
    arrowcompressall = 0xf044,
    arrowdown = 0xf045,
    arrowdownbold = 0xf046,
    arrowdownboldcircle = 0xf047,
    arrowdownboldcircleoutline = 0xf048,
    arrowdownboldhexagonoutline = 0xf049,
    arrowdownbox = 0xf6BF,
    arrowdowndropcircle = 0xf04A,
    arrowdowndropcircleoutline = 0xf04B,
    arrowexpand = 0xf616,
    arrowexpandall = 0xf04C,
    arrowleft = 0xf04D,
    arrowleftbold = 0xf04E,
    arrowleftboldcircle = 0xf04F,
    arrowleftboldcircleoutline = 0xf050,
    arrowleftboldhexagonoutline = 0xf051,
    arrowleftbox = 0xf6C0,
    arrowleftdropcircle = 0xf052,
    arrowleftdropcircleoutline = 0xf053,
    arrowright = 0xf054,
    arrowrightbold = 0xf055,
    arrowrightboldcircle = 0xf056,
    arrowrightboldcircleoutline = 0xf057,
    arrowrightboldhexagonoutline = 0xf058,
    arrowrightbox = 0xf6C1,
    arrowrightdropcircle = 0xf059,
    arrowrightdropcircleoutline = 0xf05A,
    arrowtopleft = 0xf05B,
    arrowtopright = 0xf05C,
    arrowup = 0xf05D,
    arrowupbold = 0xf05E,
    arrowupboldcircle = 0xf05F,
    arrowupboldcircleoutline = 0xf060,
    arrowupboldhexagonoutline = 0xf061,
    arrowupbox = 0xf6C2,
    arrowupdropcircle = 0xf062,
    arrowupdropcircleoutline = 0xf063,
    assistant = 0xf064,
    asterisk = 0xf6C3,
    at = 0xf065,
    attachment = 0xf066,
    audiobook = 0xf067,
    autofix = 0xf068,
    autoupload = 0xf069,
    autorenew = 0xf06A,
    avtimer = 0xf06B,
    baby = 0xf06C,
    babybuggy = 0xf68E,
    backburger = 0xf06D,
    backspace = 0xf06E,
    backuprestore = 0xf06F,
    bandcamp = 0xf674,
    bank = 0xf070,
    barcode = 0xf071,
    barcodescan = 0xf072,
    barley = 0xf073,
    barrel = 0xf074,
    basecamp = 0xf075,
    basket = 0xf076,
    basketfill = 0xf077,
    basketunfill = 0xf078,
    battery = 0xf079,
    battery10 = 0xf07A,
    battery20 = 0xf07B,
    battery30 = 0xf07C,
    battery40 = 0xf07D,
    battery50 = 0xf07E,
    battery60 = 0xf07F,
    battery70 = 0xf080,
    battery80 = 0xf081,
    battery90 = 0xf082,
    batteryalert = 0xf083,
    batterycharging = 0xf084,
    batterycharging100 = 0xf085,
    batterycharging20 = 0xf086,
    batterycharging30 = 0xf087,
    batterycharging40 = 0xf088,
    batterycharging60 = 0xf089,
    batterycharging80 = 0xf08A,
    batterycharging90 = 0xf08B,
    batteryminus = 0xf08C,
    batterynegative = 0xf08D,
    batteryoutline = 0xf08E,
    batteryplus = 0xf08F,
    batterypositive = 0xf090,
    batteryunknown = 0xf091,
    beach = 0xf092,
    beaker = 0xf68F,
    beats = 0xf097,
    beer = 0xf098,
    behance = 0xf099,
    bell = 0xf09A,
    belloff = 0xf09B,
    belloutline = 0xf09C,
    bellplus = 0xf09D,
    bellring = 0xf09E,
    bellringoutline = 0xf09F,
    bellsleep = 0xf0A0,
    beta = 0xf0A1,
    bible = 0xf0A2,
    bike = 0xf0A3,
    bing = 0xf0A4,
    binoculars = 0xf0A5,
    bio = 0xf0A6,
    biohazard = 0xf0A7,
    bitbucket = 0xf0A8,
    blackmesa = 0xf0A9,
    blackberry = 0xf0AA,
    blender = 0xf0AB,
    blinds = 0xf0AC,
    blockhelper = 0xf0AD,
    blogger = 0xf0AE,
    bluetooth = 0xf0AF,
    bluetoothaudio = 0xf0B0,
    bluetoothconnect = 0xf0B1,
    bluetoothoff = 0xf0B2,
    bluetoothsettings = 0xf0B3,
    bluetoothtransfer = 0xf0B4,
    blur = 0xf0B5,
    blurlinear = 0xf0B6,
    bluroff = 0xf0B7,
    blurradial = 0xf0B8,
    bomb = 0xf690,
    bomboff = 0xf6C4,
    bone = 0xf0B9,
    book = 0xf0BA,
    bookminus = 0xf5D9,
    bookmultiple = 0xf0BB,
    bookmultiplevariant = 0xf0BC,
    bookopen = 0xf0BD,
    bookopenpagevariant = 0xf5DA,
    bookopenvariant = 0xf0BE,
    bookplus = 0xf5DB,
    bookvariant = 0xf0BF,
    bookmark = 0xf0C0,
    bookmarkcheck = 0xf0C1,
    bookmarkmusic = 0xf0C2,
    bookmarkoutline = 0xf0C3,
    bookmarkplus = 0xf0C5,
    bookmarkplusoutline = 0xf0C4,
    bookmarkremove = 0xf0C6,
    boombox = 0xf5DC,
    bootstrap = 0xf6C5,
    borderall = 0xf0C7,
    borderbottom = 0xf0C8,
    bordercolor = 0xf0C9,
    borderhorizontal = 0xf0CA,
    borderinside = 0xf0CB,
    borderleft = 0xf0CC,
    bordernone = 0xf0CD,
    borderoutside = 0xf0CE,
    borderright = 0xf0CF,
    borderstyle = 0xf0D0,
    bordertop = 0xf0D1,
    bordervertical = 0xf0D2,
    bowtie = 0xf677,
    bowl = 0xf617,
    bowling = 0xf0D3,
    box = 0xf0D4,
    boxcutter = 0xf0D5,
    boxshadow = 0xf637,
    bridge = 0xf618,
    briefcase = 0xf0D6,
    briefcasecheck = 0xf0D7,
    briefcasedownload = 0xf0D8,
    briefcaseupload = 0xf0D9,
    brightness1 = 0xf0DA,
    brightness2 = 0xf0DB,
    brightness3 = 0xf0DC,
    brightness4 = 0xf0DD,
    brightness5 = 0xf0DE,
    brightness6 = 0xf0DF,
    brightness7 = 0xf0E0,
    brightnessauto = 0xf0E1,
    broom = 0xf0E2,
    brush = 0xf0E3,
    buffer = 0xf619,
    bug = 0xf0E4,
    bulletinboard = 0xf0E5,
    bullhorn = 0xf0E6,
    bullseye = 0xf5DD,
    burstmode = 0xf5DE,
    bus = 0xf0E7,
    cached = 0xf0E8,
    cake = 0xf0E9,
    cakelayered = 0xf0EA,
    cakevariant = 0xf0EB,
    calculator = 0xf0EC,
    calendar = 0xf0ED,
    calendarblank = 0xf0EE,
    calendarcheck = 0xf0EF,
    calendarclock = 0xf0F0,
    calendarmultiple = 0xf0F1,
    calendarmultiplecheck = 0xf0F2,
    calendarplus = 0xf0F3,
    calendarquestion = 0xf691,
    calendarrange = 0xf678,
    calendarremove = 0xf0F4,
    calendartext = 0xf0F5,
    calendartoday = 0xf0F6,
    callmade = 0xf0F7,
    callmerge = 0xf0F8,
    callmissed = 0xf0F9,
    callreceived = 0xf0FA,
    callsplit = 0xf0FB,
    camcorder = 0xf0FC,
    camcorderbox = 0xf0FD,
    camcorderboxoff = 0xf0FE,
    camcorderoff = 0xf0FF,
    camera = 0xf100,
    cameraburst = 0xf692,
    cameraenhance = 0xf101,
    camerafront = 0xf102,
    camerafrontvariant = 0xf103,
    camerairis = 0xf104,
    cameraoff = 0xf5DF,
    camerapartymode = 0xf105,
    camerarear = 0xf106,
    camerarearvariant = 0xf107,
    cameraswitch = 0xf108,
    cameratimer = 0xf109,
    candle = 0xf5E2,
    candycane = 0xf10A,
    car = 0xf10B,
    carbattery = 0xf10C,
    carconnected = 0xf10D,
    carwash = 0xf10E,
    cards = 0xf638,
    cardsoutline = 0xf639,
    cardsplayingoutline = 0xf63A,
    cardsvariant = 0xf6C6,
    carrot = 0xf10F,
    cart = 0xf110,
    cartoff = 0xf66B,
    cartoutline = 0xf111,
    cartplus = 0xf112,
    casesensitivealt = 0xf113,
    cash = 0xf114,
    cash100 = 0xf115,
    cashmultiple = 0xf116,
    cashusd = 0xf117,
    cast = 0xf118,
    castconnected = 0xf119,
    castle = 0xf11A,
    cat = 0xf11B,
    cellphone = 0xf11C,
    cellphoneandroid = 0xf11D,
    cellphonebasic = 0xf11E,
    cellphonedock = 0xf11F,
    cellphoneiphone = 0xf120,
    cellphonelink = 0xf121,
    cellphonelinkoff = 0xf122,
    cellphonesettings = 0xf123,
    certificate = 0xf124,
    chairschool = 0xf125,
    chartarc = 0xf126,
    chartareaspline = 0xf127,
    chartbar = 0xf128,
    chartbubble = 0xf5E3,
    chartgantt = 0xf66C,
    charthistogram = 0xf129,
    chartline = 0xf12A,
    chartpie = 0xf12B,
    chartscatterplothexbin = 0xf66D,
    charttimeline = 0xf66E,
    check = 0xf12C,
    checkall = 0xf12D,
    checkcircle = 0xf5E0,
    checkcircleoutline = 0xf5E1,
    checkboxblank = 0xf12E,
    checkboxblankcircle = 0xf12F,
    checkboxblankcircleoutline = 0xf130,
    checkboxblankoutline = 0xf131,
    checkboxmarked = 0xf132,
    checkboxmarkedcircle = 0xf133,
    checkboxmarkedcircleoutline = 0xf134,
    checkboxmarkedoutline = 0xf135,
    checkboxmultipleblank = 0xf136,
    checkboxmultipleblankcircle = 0xf63B,
    checkboxmultipleblankcircleoutline = 0xf63C,
    checkboxmultipleblankoutline = 0xf137,
    checkboxmultiplemarked = 0xf138,
    checkboxmultiplemarkedcircle = 0xf63D,
    checkboxmultiplemarkedcircleoutline = 0xf63E,
    checkboxmultiplemarkedoutline = 0xf139,
    checkerboard = 0xf13A,
    chemicalweapon = 0xf13B,
    chevrondoubledown = 0xf13C,
    chevrondoubleleft = 0xf13D,
    chevrondoubleright = 0xf13E,
    chevrondoubleup = 0xf13F,
    chevrondown = 0xf140,
    chevronleft = 0xf141,
    chevronright = 0xf142,
    chevronup = 0xf143,
    chip = 0xf61A,
    church = 0xf144,
    ciscowebex = 0xf145,
    city = 0xf146,
    clipboard = 0xf147,
    clipboardaccount = 0xf148,
    clipboardalert = 0xf149,
    clipboardarrowdown = 0xf14A,
    clipboardarrowleft = 0xf14B,
    clipboardcheck = 0xf14C,
    clipboardflow = 0xf6C7,
    clipboardoutline = 0xf14D,
    clipboardtext = 0xf14E,
    clippy = 0xf14F,
    clock = 0xf150,
    clockalert = 0xf5CE,
    clockend = 0xf151,
    clockfast = 0xf152,
    clockin = 0xf153,
    clockout = 0xf154,
    clockstart = 0xf155,
    close = 0xf156,
    closebox = 0xf157,
    closeboxoutline = 0xf158,
    closecircle = 0xf159,
    closecircleoutline = 0xf15A,
    closenetwork = 0xf15B,
    closeoctagon = 0xf15C,
    closeoctagonoutline = 0xf15D,
    closeoutline = 0xf6C8,
    closedcaption = 0xf15E,
    cloud = 0xf15F,
    cloudcheck = 0xf160,
    cloudcircle = 0xf161,
    clouddownload = 0xf162,
    cloudoutline = 0xf163,
    cloudoutlineoff = 0xf164,
    cloudprint = 0xf165,
    cloudprintoutline = 0xf166,
    cloudsync = 0xf63F,
    cloudupload = 0xf167,
    codearray = 0xf168,
    codebraces = 0xf169,
    codebrackets = 0xf16A,
    codeequal = 0xf16B,
    codegreaterthan = 0xf16C,
    codegreaterthanorequal = 0xf16D,
    codelessthan = 0xf16E,
    codelessthanorequal = 0xf16F,
    codenotequal = 0xf170,
    codenotequalvariant = 0xf171,
    codeparentheses = 0xf172,
    codestring = 0xf173,
    codetags = 0xf174,
    codetagscheck = 0xf693,
    codepen = 0xf175,
    coffee = 0xf176,
    coffeeoutline = 0xf6C9,
    coffeetogo = 0xf177,
    coin = 0xf178,
    coins = 0xf694,
    collage = 0xf640,
    colorhelper = 0xf179,
    comment = 0xf17A,
    commentaccount = 0xf17B,
    commentaccountoutline = 0xf17C,
    commentalert = 0xf17D,
    commentalertoutline = 0xf17E,
    commentcheck = 0xf17F,
    commentcheckoutline = 0xf180,
    commentmultipleoutline = 0xf181,
    commentoutline = 0xf182,
    commentplusoutline = 0xf183,
    commentprocessing = 0xf184,
    commentprocessingoutline = 0xf185,
    commentquestionoutline = 0xf186,
    commentremoveoutline = 0xf187,
    commenttext = 0xf188,
    commenttextoutline = 0xf189,
    compare = 0xf18A,
    compass = 0xf18B,
    compassoutline = 0xf18C,
    console = 0xf18D,
    contactmail = 0xf18E,
    contacts = 0xf6CA,
    contentcopy = 0xf18F,
    contentcut = 0xf190,
    contentduplicate = 0xf191,
    contentpaste = 0xf192,
    contentsave = 0xf193,
    contentsaveall = 0xf194,
    contentsavesettings = 0xf61B,
    contrast = 0xf195,
    contrastbox = 0xf196,
    contrastcircle = 0xf197,
    cookie = 0xf198,
    copyright = 0xf5E6,
    counter = 0xf199,
    cow = 0xf19A,
    creation = 0xf1C9,
    creditcard = 0xf19B,
    creditcardmultiple = 0xf19C,
    creditcardoff = 0xf5E4,
    creditcardplus = 0xf675,
    creditcardscan = 0xf19D,
    crop = 0xf19E,
    cropfree = 0xf19F,
    croplandscape = 0xf1A0,
    cropportrait = 0xf1A1,
    croprotate = 0xf695,
    cropsquare = 0xf1A2,
    crosshairs = 0xf1A3,
    crosshairsgps = 0xf1A4,
    crown = 0xf1A5,
    cube = 0xf1A6,
    cubeoutline = 0xf1A7,
    cubesend = 0xf1A8,
    cubeunfolded = 0xf1A9,
    cup = 0xf1AA,
    cupoff = 0xf5E5,
    cupwater = 0xf1AB,
    currencybtc = 0xf1AC,
    currencyeur = 0xf1AD,
    currencygbp = 0xf1AE,
    currencyinr = 0xf1AF,
    currencyngn = 0xf1B0,
    currencyrub = 0xf1B1,
    currencytry = 0xf1B2,
    currencyusd = 0xf1B3,
    currencyusdoff = 0xf679,
    cursordefault = 0xf1B4,
    cursordefaultoutline = 0xf1B5,
    cursormove = 0xf1B6,
    cursorpointer = 0xf1B7,
    cursortext = 0xf5E7,
    database = 0xf1B8,
    databaseminus = 0xf1B9,
    databaseplus = 0xf1BA,
    debugstepinto = 0xf1BB,
    debugstepout = 0xf1BC,
    debugstepover = 0xf1BD,
    decimaldecrease = 0xf1BE,
    decimalincrease = 0xf1BF,
    delete_ = 0xf1C0,
    deletecircle = 0xf682,
    deleteempty = 0xf6CB,
    deleteforever = 0xf5E8,
    deletesweep = 0xf5E9,
    deletevariant = 0xf1C1,
    delta = 0xf1C2,
    deskphone = 0xf1C3,
    desktopmac = 0xf1C4,
    desktoptower = 0xf1C5,
    details = 0xf1C6,
    developerboard = 0xf696,
    deviantart = 0xf1C7,
    dialpad = 0xf61C,
    diamond = 0xf1C8,
    dice1 = 0xf1CA,
    dice2 = 0xf1CB,
    dice3 = 0xf1CC,
    dice4 = 0xf1CD,
    dice5 = 0xf1CE,
    dice6 = 0xf1CF,
    diced20 = 0xf5EA,
    diced4 = 0xf5EB,
    diced6 = 0xf5EC,
    diced8 = 0xf5ED,
    dictionary = 0xf61D,
    directions = 0xf1D0,
    directionsfork = 0xf641,
    discord = 0xf66F,
    disk = 0xf5EE,
    diskalert = 0xf1D1,
    disqus = 0xf1D2,
    disqusoutline = 0xf1D3,
    division = 0xf1D4,
    divisionbox = 0xf1D5,
    dna = 0xf683,
    dns = 0xf1D6,
    donotdisturb = 0xf697,
    donotdisturboff = 0xf698,
    dolby = 0xf6B2,
    domain = 0xf1D7,
    dotshorizontal = 0xf1D8,
    dotsvertical = 0xf1D9,
    douban = 0xf699,
    download = 0xf1DA,
    drag = 0xf1DB,
    draghorizontal = 0xf1DC,
    dragvertical = 0xf1DD,
    drawing = 0xf1DE,
    drawingbox = 0xf1DF,
    dribbble = 0xf1E0,
    dribbblebox = 0xf1E1,
    drone = 0xf1E2,
    dropbox = 0xf1E3,
    drupal = 0xf1E4,
    duck = 0xf1E5,
    dumbbell = 0xf1E6,
    earth = 0xf1E7,
    earthbox = 0xf6CC,
    earthboxoff = 0xf6CD,
    earthoff = 0xf1E8,
    edge = 0xf1E9,
    eject = 0xf1EA,
    elevationdecline = 0xf1EB,
    elevationrise = 0xf1EC,
    elevator = 0xf1ED,
    email = 0xf1EE,
    emailalert = 0xf6CE,
    emailopen = 0xf1EF,
    emailopenoutline = 0xf5EF,
    emailoutline = 0xf1F0,
    emailsecure = 0xf1F1,
    emailvariant = 0xf5F0,
    emby = 0xf6B3,
    emoticon = 0xf1F2,
    emoticoncool = 0xf1F3,
    emoticondead = 0xf69A,
    emoticondevil = 0xf1F4,
    emoticonexcited = 0xf69B,
    emoticonhappy = 0xf1F5,
    emoticonneutral = 0xf1F6,
    emoticonpoop = 0xf1F7,
    emoticonsad = 0xf1F8,
    emoticontongue = 0xf1F9,
    engine = 0xf1FA,
    engineoutline = 0xf1FB,
    equal = 0xf1FC,
    equalbox = 0xf1FD,
    eraser = 0xf1FE,
    eraservariant = 0xf642,
    escalator = 0xf1FF,
    ethernet = 0xf200,
    ethernetcable = 0xf201,
    ethernetcableoff = 0xf202,
    etsy = 0xf203,
    evstation = 0xf5F1,
    evernote = 0xf204,
    exclamation = 0xf205,
    exittoapp = 0xf206,
    export_ = 0xf207,
    eye = 0xf208,
    eyeoff = 0xf209,
    eyeoutline = 0xf6CF,
    eyeoutlineoff = 0xf6D0,
    eyedropper = 0xf20A,
    eyedroppervariant = 0xf20B,
    face = 0xf643,
    faceprofile = 0xf644,
    facebook = 0xf20C,
    facebookbox = 0xf20D,
    facebookmessenger = 0xf20E,
    factory = 0xf20F,
    fan = 0xf210,
    fastforward = 0xf211,
    fastforwardoutline = 0xf6D1,
    fax = 0xf212,
    feather = 0xf6D2,
    ferry = 0xf213,
    file = 0xf214,
    filechart = 0xf215,
    filecheck = 0xf216,
    filecloud = 0xf217,
    filedelimited = 0xf218,
    filedocument = 0xf219,
    filedocumentbox = 0xf21A,
    fileexcel = 0xf21B,
    fileexcelbox = 0xf21C,
    fileexport = 0xf21D,
    filefind = 0xf21E,
    filehidden = 0xf613,
    fileimage = 0xf21F,
    fileimport = 0xf220,
    filelock = 0xf221,
    filemultiple = 0xf222,
    filemusic = 0xf223,
    fileoutline = 0xf224,
    filepdf = 0xf225,
    filepdfbox = 0xf226,
    filepowerpoint = 0xf227,
    filepowerpointbox = 0xf228,
    filepresentationbox = 0xf229,
    filerestore = 0xf670,
    filesend = 0xf22A,
    filetree = 0xf645,
    filevideo = 0xf22B,
    fileword = 0xf22C,
    filewordbox = 0xf22D,
    filexml = 0xf22E,
    film = 0xf22F,
    filmstrip = 0xf230,
    filmstripoff = 0xf231,
    filter = 0xf232,
    filteroutline = 0xf233,
    filterremove = 0xf234,
    filterremoveoutline = 0xf235,
    filtervariant = 0xf236,
    findreplace = 0xf6D3,
    fingerprint = 0xf237,
    fire = 0xf238,
    firefox = 0xf239,
    fish = 0xf23A,
    flag = 0xf23B,
    flagcheckered = 0xf23C,
    flagoutline = 0xf23D,
    flagoutlinevariant = 0xf23E,
    flagtriangle = 0xf23F,
    flagvariant = 0xf240,
    flash = 0xf241,
    flashauto = 0xf242,
    flashoff = 0xf243,
    flashoutline = 0xf6D4,
    flashredeye = 0xf67A,
    flashlight = 0xf244,
    flashlightoff = 0xf245,
    flask = 0xf093,
    flaskempty = 0xf094,
    flaskemptyoutline = 0xf095,
    flaskoutline = 0xf096,
    flattr = 0xf246,
    fliptoback = 0xf247,
    fliptofront = 0xf248,
    floppy = 0xf249,
    flower = 0xf24A,
    folder = 0xf24B,
    folderaccount = 0xf24C,
    folderdownload = 0xf24D,
    foldergoogledrive = 0xf24E,
    folderimage = 0xf24F,
    folderlock = 0xf250,
    folderlockopen = 0xf251,
    foldermove = 0xf252,
    foldermultiple = 0xf253,
    foldermultipleimage = 0xf254,
    foldermultipleoutline = 0xf255,
    folderoutline = 0xf256,
    folderplus = 0xf257,
    folderremove = 0xf258,
    folderstar = 0xf69C,
    folderupload = 0xf259,
    fontawesome = 0xf03A,
    food = 0xf25A,
    foodapple = 0xf25B,
    foodforkdrink = 0xf5F2,
    foodoff = 0xf5F3,
    foodvariant = 0xf25C,
    football = 0xf25D,
    footballaustralian = 0xf25E,
    footballhelmet = 0xf25F,
    formataligncenter = 0xf260,
    formatalignjustify = 0xf261,
    formatalignleft = 0xf262,
    formatalignright = 0xf263,
    formatannotationplus = 0xf646,
    formatbold = 0xf264,
    formatclear = 0xf265,
    formatcolorfill = 0xf266,
    formatcolortext = 0xf69D,
    formatfloatcenter = 0xf267,
    formatfloatleft = 0xf268,
    formatfloatnone = 0xf269,
    formatfloatright = 0xf26A,
    formatfont = 0xf6D5,
    formatheader1 = 0xf26B,
    formatheader2 = 0xf26C,
    formatheader3 = 0xf26D,
    formatheader4 = 0xf26E,
    formatheader5 = 0xf26F,
    formatheader6 = 0xf270,
    formatheaderdecrease = 0xf271,
    formatheaderequal = 0xf272,
    formatheaderincrease = 0xf273,
    formatheaderpound = 0xf274,
    formathorizontalaligncenter = 0xf61E,
    formathorizontalalignleft = 0xf61F,
    formathorizontalalignright = 0xf620,
    formatindentdecrease = 0xf275,
    formatindentincrease = 0xf276,
    formatitalic = 0xf277,
    formatlinespacing = 0xf278,
    formatlinestyle = 0xf5C8,
    formatlineweight = 0xf5C9,
    formatlistbulleted = 0xf279,
    formatlistbulletedtype = 0xf27A,
    formatlistnumbers = 0xf27B,
    formatpagebreak = 0xf6D6,
    formatpaint = 0xf27C,
    formatparagraph = 0xf27D,
    formatpilcrow = 0xf6D7,
    formatquote = 0xf27E,
    formatrotate90 = 0xf6A9,
    formatsection = 0xf69E,
    formatsize = 0xf27F,
    formatstrikethrough = 0xf280,
    formatstrikethroughvariant = 0xf281,
    formatsubscript = 0xf282,
    formatsuperscript = 0xf283,
    formattext = 0xf284,
    formattextdirectionltor = 0xf285,
    formattextdirectionrtol = 0xf286,
    formattitle = 0xf5F4,
    formatunderline = 0xf287,
    formatverticalalignbottom = 0xf621,
    formatverticalaligncenter = 0xf622,
    formatverticalaligntop = 0xf623,
    formatwrapinline = 0xf288,
    formatwrapsquare = 0xf289,
    formatwraptight = 0xf28A,
    formatwraptopbottom = 0xf28B,
    forum = 0xf28C,
    forward = 0xf28D,
    foursquare = 0xf28E,
    fridge = 0xf28F,
    fridgefilled = 0xf290,
    fridgefilledbottom = 0xf291,
    fridgefilledtop = 0xf292,
    fullscreen = 0xf293,
    fullscreenexit = 0xf294,
    function = 0xf295,
    gamepad = 0xf296,
    gamepadvariant = 0xf297,
    garage = 0xf6D8,
    garageopen = 0xf6D9,
    gascylinder = 0xf647,
    gasstation = 0xf298,
    gate = 0xf299,
    gauge = 0xf29A,
    gavel = 0xf29B,
    genderfemale = 0xf29C,
    gendermale = 0xf29D,
    gendermalefemale = 0xf29E,
    gendertransgender = 0xf29F,
    ghost = 0xf2A0,
    gift = 0xf2A1,
    git = 0xf2A2,
    githubbox = 0xf2A3,
    githubcircle = 0xf2A4,
    githubface = 0xf6DA,
    glassflute = 0xf2A5,
    glassmug = 0xf2A6,
    glassstange = 0xf2A7,
    glasstulip = 0xf2A8,
    glassdoor = 0xf2A9,
    glasses = 0xf2AA,
    gmail = 0xf2AB,
    gnome = 0xf2AC,
    gondola = 0xf685,
    google = 0xf2AD,
    googlecardboard = 0xf2AE,
    googlechrome = 0xf2AF,
    googlecircles = 0xf2B0,
    googlecirclescommunities = 0xf2B1,
    googlecirclesextended = 0xf2B2,
    googlecirclesgroup = 0xf2B3,
    googlecontroller = 0xf2B4,
    googlecontrolleroff = 0xf2B5,
    googledrive = 0xf2B6,
    googleearth = 0xf2B7,
    googleglass = 0xf2B8,
    googlekeep = 0xf6DB,
    googlemaps = 0xf5F5,
    googlenearby = 0xf2B9,
    googlepages = 0xf2BA,
    googlephotos = 0xf6DC,
    googlephysicalweb = 0xf2BB,
    googleplay = 0xf2BC,
    googleplus = 0xf2BD,
    googleplusbox = 0xf2BE,
    googletranslate = 0xf2BF,
    googlewallet = 0xf2C0,
    gradient = 0xf69F,
    greasepencil = 0xf648,
    grid = 0xf2C1,
    gridoff = 0xf2C2,
    group = 0xf2C3,
    guitarelectric = 0xf2C4,
    guitarpick = 0xf2C5,
    guitarpickoutline = 0xf2C6,
    hackernews = 0xf624,
    hamburger = 0xf684,
    handpointingright = 0xf2C7,
    hanger = 0xf2C8,
    hangouts = 0xf2C9,
    harddisk = 0xf2CA,
    headphones = 0xf2CB,
    headphonesbox = 0xf2CC,
    headphonessettings = 0xf2CD,
    headset = 0xf2CE,
    headsetdock = 0xf2CF,
    headsetoff = 0xf2D0,
    heart = 0xf2D1,
    heartbox = 0xf2D2,
    heartboxoutline = 0xf2D3,
    heartbroken = 0xf2D4,
    hearthalfoutline = 0xf6DD,
    hearthalfpart = 0xf6DE,
    hearthalfpartoutline = 0xf6DF,
    heartoutline = 0xf2D5,
    heartpulse = 0xf5F6,
    help = 0xf2D6,
    helpcircle = 0xf2D7,
    helpcircleoutline = 0xf625,
    hexagon = 0xf2D8,
    hexagonmultiple = 0xf6E0,
    hexagonoutline = 0xf2D9,
    highway = 0xf5F7,
    history = 0xf2DA,
    hololens = 0xf2DB,
    home = 0xf2DC,
    homemapmarker = 0xf5F8,
    homemodern = 0xf2DD,
    homeoutline = 0xf6A0,
    homevariant = 0xf2DE,
    hook = 0xf6E1,
    hookoff = 0xf6E2,
    hops = 0xf2DF,
    hospital = 0xf2E0,
    hospitalbuilding = 0xf2E1,
    hospitalmarker = 0xf2E2,
    hotel = 0xf2E3,
    houzz = 0xf2E4,
    houzzbox = 0xf2E5,
    human = 0xf2E6,
    humanchild = 0xf2E7,
    humanfemale = 0xf649,
    humangreeting = 0xf64A,
    humanhandsdown = 0xf64B,
    humanhandsup = 0xf64C,
    humanmale = 0xf64D,
    humanmalefemale = 0xf2E8,
    humanpregnant = 0xf5CF,
    image = 0xf2E9,
    imagealbum = 0xf2EA,
    imagearea = 0xf2EB,
    imageareaclose = 0xf2EC,
    imagebroken = 0xf2ED,
    imagebrokenvariant = 0xf2EE,
    imagefilter = 0xf2EF,
    imagefilterblackwhite = 0xf2F0,
    imagefiltercenterfocus = 0xf2F1,
    imagefiltercenterfocusweak = 0xf2F2,
    imagefilterdrama = 0xf2F3,
    imagefilterframes = 0xf2F4,
    imagefilterhdr = 0xf2F5,
    imagefilternone = 0xf2F6,
    imagefiltertiltshift = 0xf2F7,
    imagefiltervintage = 0xf2F8,
    imagemultiple = 0xf2F9,
    import = 0xf2FA,
    inbox = 0xf686,
    inboxarrowdown = 0xf2FB,
    inboxarrowup = 0xf3D1,
    incognito = 0xf5F9,
    infinity = 0xf6E3,
    information = 0xf2FC,
    informationoutline = 0xf2FD,
    informationvariant = 0xf64E,
    instagram = 0xf2FE,
    instapaper = 0xf2FF,
    internetexplorer = 0xf300,
    invertcolors = 0xf301,
    itunes = 0xf676,
    jeepney = 0xf302,
    jira = 0xf303,
    jsfiddle = 0xf304,
    json = 0xf626,
    keg = 0xf305,
    kettle = 0xf5FA,
    key = 0xf306,
    keychange = 0xf307,
    keyminus = 0xf308,
    keyplus = 0xf309,
    keyremove = 0xf30A,
    keyvariant = 0xf30B,
    keyboard = 0xf30C,
    keyboardbackspace = 0xf30D,
    keyboardcaps = 0xf30E,
    keyboardclose = 0xf30F,
    keyboardoff = 0xf310,
    keyboardreturn = 0xf311,
    keyboardtab = 0xf312,
    keyboardvariant = 0xf313,
    kodi = 0xf314,
    label = 0xf315,
    labeloutline = 0xf316,
    lambda = 0xf627,
    lamp = 0xf6B4,
    lan = 0xf317,
    lanconnect = 0xf318,
    landisconnect = 0xf319,
    lanpending = 0xf31A,
    languagec = 0xf671,
    languagecpp = 0xf672,
    languagecsharp = 0xf31B,
    languagecss3 = 0xf31C,
    languagehtml5 = 0xf31D,
    languagejavascript = 0xf31E,
    languagephp = 0xf31F,
    languagepython = 0xf320,
    languagepythontext = 0xf321,
    languageswift = 0xf6E4,
    languagetypescript = 0xf6E5,
    laptop = 0xf322,
    laptopchromebook = 0xf323,
    laptopmac = 0xf324,
    laptopoff = 0xf6E6,
    laptopwindows = 0xf325,
    lastfm = 0xf326,
    launch = 0xf327,
    layers = 0xf328,
    layersoff = 0xf329,
    leadpencil = 0xf64F,
    leaf = 0xf32A,
    ledoff = 0xf32B,
    ledon = 0xf32C,
    ledoutline = 0xf32D,
    ledvariantoff = 0xf32E,
    ledvarianton = 0xf32F,
    ledvariantoutline = 0xf330,
    library = 0xf331,
    librarybooks = 0xf332,
    librarymusic = 0xf333,
    libraryplus = 0xf334,
    lightbulb = 0xf335,
    lightbulbon = 0xf6E7,
    lightbulbonoutline = 0xf6E8,
    lightbulboutline = 0xf336,
    link = 0xf337,
    linkoff = 0xf338,
    linkvariant = 0xf339,
    linkvariantoff = 0xf33A,
    linkedin = 0xf33B,
    linkedinbox = 0xf33C,
    linux = 0xf33D,
    lock = 0xf33E,
    lockopen = 0xf33F,
    lockopenoutline = 0xf340,
    lockoutline = 0xf341,
    lockpattern = 0xf6E9,
    lockplus = 0xf5FB,
    login = 0xf342,
    loginvariant = 0xf5FC,
    logout = 0xf343,
    logoutvariant = 0xf5FD,
    looks = 0xf344,
    loop = 0xf6EA,
    loupe = 0xf345,
    lumx = 0xf346,
    magnet = 0xf347,
    magneton = 0xf348,
    magnify = 0xf349,
    magnifyminus = 0xf34A,
    magnifyminusoutline = 0xf6EB,
    magnifyplus = 0xf34B,
    magnifyplusoutline = 0xf6EC,
    mailru = 0xf34C,
    mailbox = 0xf6ED,
    map = 0xf34D,
    mapmarker = 0xf34E,
    mapmarkercircle = 0xf34F,
    mapmarkerminus = 0xf650,
    mapmarkermultiple = 0xf350,
    mapmarkeroff = 0xf351,
    mapmarkerplus = 0xf651,
    mapmarkerradius = 0xf352,
    margin = 0xf353,
    markdown = 0xf354,
    marker = 0xf652,
    markercheck = 0xf355,
    martini = 0xf356,
    materialui = 0xf357,
    mathcompass = 0xf358,
    matrix = 0xf628,
    maxcdn = 0xf359,
    medicalbag = 0xf6EE,
    medium = 0xf35A,
    memory = 0xf35B,
    menu = 0xf35C,
    menudown = 0xf35D,
    menudownoutline = 0xf6B5,
    menuleft = 0xf35E,
    menuright = 0xf35F,
    menuup = 0xf360,
    menuupoutline = 0xf6B6,
    message = 0xf361,
    messagealert = 0xf362,
    messagebulleted = 0xf6A1,
    messagebulletedoff = 0xf6A2,
    messagedraw = 0xf363,
    messageimage = 0xf364,
    messageoutline = 0xf365,
    messageplus = 0xf653,
    messageprocessing = 0xf366,
    messagereply = 0xf367,
    messagereplytext = 0xf368,
    messagesettings = 0xf6EF,
    messagesettingsvariant = 0xf6F0,
    messagetext = 0xf369,
    messagetextoutline = 0xf36A,
    messagevideo = 0xf36B,
    meteor = 0xf629,
    microphone = 0xf36C,
    microphoneoff = 0xf36D,
    microphoneoutline = 0xf36E,
    microphonesettings = 0xf36F,
    microphonevariant = 0xf370,
    microphonevariantoff = 0xf371,
    microscope = 0xf654,
    microsoft = 0xf372,
    minecraft = 0xf373,
    minus = 0xf374,
    minusbox = 0xf375,
    minusboxoutline = 0xf6F1,
    minuscircle = 0xf376,
    minuscircleoutline = 0xf377,
    minusnetwork = 0xf378,
    mixcloud = 0xf62A,
    monitor = 0xf379,
    monitormultiple = 0xf37A,
    more = 0xf37B,
    motorbike = 0xf37C,
    mouse = 0xf37D,
    mouseoff = 0xf37E,
    mousevariant = 0xf37F,
    mousevariantoff = 0xf380,
    moveresize = 0xf655,
    moveresizevariant = 0xf656,
    movie = 0xf381,
    multiplication = 0xf382,
    multiplicationbox = 0xf383,
    musicbox = 0xf384,
    musicboxoutline = 0xf385,
    musiccircle = 0xf386,
    musicnote = 0xf387,
    musicnotebluetooth = 0xf5FE,
    musicnotebluetoothoff = 0xf5FF,
    musicnoteeighth = 0xf388,
    musicnotehalf = 0xf389,
    musicnoteoff = 0xf38A,
    musicnotequarter = 0xf38B,
    musicnotesixteenth = 0xf38C,
    musicnotewhole = 0xf38D,
    nature = 0xf38E,
    naturepeople = 0xf38F,
    navigation = 0xf390,
    nearme = 0xf5CD,
    needle = 0xf391,
    nestprotect = 0xf392,
    nestthermostat = 0xf393,
    network = 0xf6F2,
    networkdownload = 0xf6F3,
    networkquestion = 0xf6F4,
    networkupload = 0xf6F5,
    newbox = 0xf394,
    newspaper = 0xf395,
    nfc = 0xf396,
    nfctap = 0xf397,
    nfcvariant = 0xf398,
    nodejs = 0xf399,
    note = 0xf39A,
    notemultiple = 0xf6B7,
    notemultipleoutline = 0xf6B8,
    noteoutline = 0xf39B,
    noteplus = 0xf39C,
    noteplusoutline = 0xf39D,
    notetext = 0xf39E,
    notificationclearall = 0xf39F,
    npm = 0xf6F6,
    nuke = 0xf6A3,
    numeric = 0xf3A0,
    numeric0box = 0xf3A1,
    numeric0boxmultipleoutline = 0xf3A2,
    numeric0boxoutline = 0xf3A3,
    numeric1box = 0xf3A4,
    numeric1boxmultipleoutline = 0xf3A5,
    numeric1boxoutline = 0xf3A6,
    numeric2box = 0xf3A7,
    numeric2boxmultipleoutline = 0xf3A8,
    numeric2boxoutline = 0xf3A9,
    numeric3box = 0xf3AA,
    numeric3boxmultipleoutline = 0xf3AB,
    numeric3boxoutline = 0xf3AC,
    numeric4box = 0xf3AD,
    numeric4boxmultipleoutline = 0xf3AE,
    numeric4boxoutline = 0xf3AF,
    numeric5box = 0xf3B0,
    numeric5boxmultipleoutline = 0xf3B1,
    numeric5boxoutline = 0xf3B2,
    numeric6box = 0xf3B3,
    numeric6boxmultipleoutline = 0xf3B4,
    numeric6boxoutline = 0xf3B5,
    numeric7box = 0xf3B6,
    numeric7boxmultipleoutline = 0xf3B7,
    numeric7boxoutline = 0xf3B8,
    numeric8box = 0xf3B9,
    numeric8boxmultipleoutline = 0xf3BA,
    numeric8boxoutline = 0xf3BB,
    numeric9box = 0xf3BC,
    numeric9boxmultipleoutline = 0xf3BD,
    numeric9boxoutline = 0xf3BE,
    numeric9plusbox = 0xf3BF,
    numeric9plusboxmultipleoutline = 0xf3C0,
    numeric9plusboxoutline = 0xf3C1,
    nut = 0xf6F7,
    nutrition = 0xf3C2,
    oar = 0xf67B,
    octagon = 0xf3C3,
    octagonoutline = 0xf3C4,
    octagram = 0xf6F8,
    odnoklassniki = 0xf3C5,
    office = 0xf3C6,
    oil = 0xf3C7,
    oiltemperature = 0xf3C8,
    omega = 0xf3C9,
    onedrive = 0xf3CA,
    opacity = 0xf5CC,
    openinapp = 0xf3CB,
    openinnew = 0xf3CC,
    openid = 0xf3CD,
    opera = 0xf3CE,
    ornament = 0xf3CF,
    ornamentvariant = 0xf3D0,
    owl = 0xf3D2,
    package = 0xf3D3,
    packagedown = 0xf3D4,
    packageup = 0xf3D5,
    packagevariant = 0xf3D6,
    packagevariantclosed = 0xf3D7,
    pagefirst = 0xf600,
    pagelast = 0xf601,
    pagelayoutbody = 0xf6F9,
    pagelayoutfooter = 0xf6FA,
    pagelayoutheader = 0xf6FB,
    pagelayoutsidebarleft = 0xf6FC,
    pagelayoutsidebarright = 0xf6FD,
    palette = 0xf3D8,
    paletteadvanced = 0xf3D9,
    panda = 0xf3DA,
    pandora = 0xf3DB,
    panorama = 0xf3DC,
    panoramafisheye = 0xf3DD,
    panoramahorizontal = 0xf3DE,
    panoramavertical = 0xf3DF,
    panoramawideangle = 0xf3E0,
    papercutvertical = 0xf3E1,
    paperclip = 0xf3E2,
    parking = 0xf3E3,
    pause = 0xf3E4,
    pausecircle = 0xf3E5,
    pausecircleoutline = 0xf3E6,
    pauseoctagon = 0xf3E7,
    pauseoctagonoutline = 0xf3E8,
    paw = 0xf3E9,
    pawoff = 0xf657,
    pen = 0xf3EA,
    pencil = 0xf3EB,
    pencilbox = 0xf3EC,
    pencilboxoutline = 0xf3ED,
    pencilcircle = 0xf6FE,
    pencillock = 0xf3EE,
    penciloff = 0xf3EF,
    pentagon = 0xf6FF,
    pentagonoutline = 0xf700,
    percent = 0xf3F0,
    pharmacy = 0xf3F1,
    phone = 0xf3F2,
    phonebluetooth = 0xf3F3,
    phoneclassic = 0xf602,
    phoneforward = 0xf3F4,
    phonehangup = 0xf3F5,
    phoneintalk = 0xf3F6,
    phoneincoming = 0xf3F7,
    phonelocked = 0xf3F8,
    phonelog = 0xf3F9,
    phoneminus = 0xf658,
    phonemissed = 0xf3FA,
    phoneoutgoing = 0xf3FB,
    phonepaused = 0xf3FC,
    phoneplus = 0xf659,
    phonesettings = 0xf3FD,
    phonevoip = 0xf3FE,
    pi = 0xf3FF,
    pibox = 0xf400,
    piano = 0xf67C,
    pig = 0xf401,
    pill = 0xf402,
    pillar = 0xf701,
    pin = 0xf403,
    pinoff = 0xf404,
    pinetree = 0xf405,
    pinetreebox = 0xf406,
    pinterest = 0xf407,
    pinterestbox = 0xf408,
    pistol = 0xf702,
    pizza = 0xf409,
    planeshield = 0xf6BA,
    play = 0xf40A,
    playboxoutline = 0xf40B,
    playcircle = 0xf40C,
    playcircleoutline = 0xf40D,
    playpause = 0xf40E,
    playprotectedcontent = 0xf40F,
    playlistcheck = 0xf5C7,
    playlistminus = 0xf410,
    playlistplay = 0xf411,
    playlistplus = 0xf412,
    playlistremove = 0xf413,
    playstation = 0xf414,
    plex = 0xf6B9,
    plus = 0xf415,
    plusbox = 0xf416,
    plusboxoutline = 0xf703,
    pluscircle = 0xf417,
    pluscirclemultipleoutline = 0xf418,
    pluscircleoutline = 0xf419,
    plusnetwork = 0xf41A,
    plusone = 0xf41B,
    plusoutline = 0xf704,
    pocket = 0xf41C,
    pokeball = 0xf41D,
    polaroid = 0xf41E,
    poll = 0xf41F,
    pollbox = 0xf420,
    polymer = 0xf421,
    pool = 0xf606,
    popcorn = 0xf422,
    pot = 0xf65A,
    potmix = 0xf65B,
    pound = 0xf423,
    poundbox = 0xf424,
    power = 0xf425,
    powerplug = 0xf6A4,
    powerplugoff = 0xf6A5,
    powersettings = 0xf426,
    powersocket = 0xf427,
    prescription = 0xf705,
    presentation = 0xf428,
    presentationplay = 0xf429,
    printer = 0xf42A,
    printer3d = 0xf42B,
    printeralert = 0xf42C,
    printersettings = 0xf706,
    priorityhigh = 0xf603,
    prioritylow = 0xf604,
    professionalhexagon = 0xf42D,
    projector = 0xf42E,
    projectorscreen = 0xf42F,
    publish = 0xf6A6,
    pulse = 0xf430,
    puzzle = 0xf431,
    qqchat = 0xf605,
    qrcode = 0xf432,
    qrcodescan = 0xf433,
    quadcopter = 0xf434,
    qualityhigh = 0xf435,
    quicktime = 0xf436,
    radar = 0xf437,
    radiator = 0xf438,
    radio = 0xf439,
    radiohandheld = 0xf43A,
    radiotower = 0xf43B,
    radioactive = 0xf43C,
    radioboxblank = 0xf43D,
    radioboxmarked = 0xf43E,
    raspberrypi = 0xf43F,
    rayend = 0xf440,
    rayendarrow = 0xf441,
    raystart = 0xf442,
    raystartarrow = 0xf443,
    raystartend = 0xf444,
    rayvertex = 0xf445,
    rdio = 0xf446,
    react = 0xf707,
    read = 0xf447,
    readability = 0xf448,
    receipt = 0xf449,
    record = 0xf44A,
    recordrec = 0xf44B,
    recycle = 0xf44C,
    reddit = 0xf44D,
    redo = 0xf44E,
    redovariant = 0xf44F,
    refresh = 0xf450,
    regex = 0xf451,
    relativescale = 0xf452,
    reload = 0xf453,
    remote = 0xf454,
    renamebox = 0xf455,
    reorderhorizontal = 0xf687,
    reordervertical = 0xf688,
    repeat = 0xf456,
    repeatoff = 0xf457,
    repeatonce = 0xf458,
    replay = 0xf459,
    reply = 0xf45A,
    replyall = 0xf45B,
    reproduction = 0xf45C,
    resizebottomright = 0xf45D,
    responsive = 0xf45E,
    restart = 0xf708,
    restore = 0xf6A7,
    rewind = 0xf45F,
    rewindoutline = 0xf709,
    rhombus = 0xf70A,
    rhombusoutline = 0xf70B,
    ribbon = 0xf460,
    road = 0xf461,
    roadvariant = 0xf462,
    robot = 0xf6A8,
    rocket = 0xf463,
    roomba = 0xf70C,
    rotate3d = 0xf464,
    rotateleft = 0xf465,
    rotateleftvariant = 0xf466,
    rotateright = 0xf467,
    rotaterightvariant = 0xf468,
    roundedcorner = 0xf607,
    routerwireless = 0xf469,
    routes = 0xf46A,
    rowing = 0xf608,
    rss = 0xf46B,
    rssbox = 0xf46C,
    ruler = 0xf46D,
    run = 0xf70D,
    runfast = 0xf46E,
    sale = 0xf46F,
    satellite = 0xf470,
    satellitevariant = 0xf471,
    saxophone = 0xf609,
    scale = 0xf472,
    scalebalance = 0xf5D1,
    scalebathroom = 0xf473,
    scanner = 0xf6AA,
    school = 0xf474,
    screenrotation = 0xf475,
    screenrotationlock = 0xf476,
    screwdriver = 0xf477,
    script = 0xf478,
    sd = 0xf479,
    seal = 0xf47A,
    searchweb = 0xf70E,
    seatflat = 0xf47B,
    seatflatangled = 0xf47C,
    seatindividualsuite = 0xf47D,
    seatlegroomextra = 0xf47E,
    seatlegroomnormal = 0xf47F,
    seatlegroomreduced = 0xf480,
    seatreclineextra = 0xf481,
    seatreclinenormal = 0xf482,
    security = 0xf483,
    securityhome = 0xf689,
    securitynetwork = 0xf484,
    select = 0xf485,
    selectall = 0xf486,
    selectinverse = 0xf487,
    selectoff = 0xf488,
    selection = 0xf489,
    send = 0xf48A,
    serialport = 0xf65C,
    server = 0xf48B,
    serverminus = 0xf48C,
    servernetwork = 0xf48D,
    servernetworkoff = 0xf48E,
    serveroff = 0xf48F,
    serverplus = 0xf490,
    serverremove = 0xf491,
    serversecurity = 0xf492,
    settings = 0xf493,
    settingsbox = 0xf494,
    shapecircleplus = 0xf65D,
    shapeplus = 0xf495,
    shapepolygonplus = 0xf65E,
    shaperectangleplus = 0xf65F,
    shapesquareplus = 0xf660,
    share = 0xf496,
    sharevariant = 0xf497,
    shield = 0xf498,
    shieldoutline = 0xf499,
    shopping = 0xf49A,
    shoppingmusic = 0xf49B,
    shovel = 0xf70F,
    shoveloff = 0xf710,
    shredder = 0xf49C,
    shuffle = 0xf49D,
    shuffledisabled = 0xf49E,
    shufflevariant = 0xf49F,
    sigma = 0xf4A0,
    sigmalower = 0xf62B,
    signcaution = 0xf4A1,
    signal = 0xf4A2,
    signal2g = 0xf711,
    signal3g = 0xf712,
    signal4g = 0xf713,
    signalhspa = 0xf714,
    signalhspaplus = 0xf715,
    signalvariant = 0xf60A,
    silverware = 0xf4A3,
    silverwarefork = 0xf4A4,
    silverwarespoon = 0xf4A5,
    silverwarevariant = 0xf4A6,
    sim = 0xf4A7,
    simalert = 0xf4A8,
    simoff = 0xf4A9,
    sitemap = 0xf4AA,
    skipbackward = 0xf4AB,
    skipforward = 0xf4AC,
    skipnext = 0xf4AD,
    skipnextcircle = 0xf661,
    skipnextcircleoutline = 0xf662,
    skipprevious = 0xf4AE,
    skippreviouscircle = 0xf663,
    skippreviouscircleoutline = 0xf664,
    skull = 0xf68B,
    skype = 0xf4AF,
    skypebusiness = 0xf4B0,
    slack = 0xf4B1,
    sleep = 0xf4B2,
    sleepoff = 0xf4B3,
    smoking = 0xf4B4,
    smokingoff = 0xf4B5,
    snapchat = 0xf4B6,
    snowflake = 0xf716,
    snowman = 0xf4B7,
    soccer = 0xf4B8,
    sofa = 0xf4B9,
    solid = 0xf68C,
    sort = 0xf4BA,
    sortalphabetical = 0xf4BB,
    sortascending = 0xf4BC,
    sortdescending = 0xf4BD,
    sortnumeric = 0xf4BE,
    sortvariant = 0xf4BF,
    soundcloud = 0xf4C0,
    sourcebranch = 0xf62C,
    sourcecommit = 0xf717,
    sourcecommitend = 0xf718,
    sourcecommitendlocal = 0xf719,
    sourcecommitlocal = 0xf71A,
    sourcecommitnextlocal = 0xf71B,
    sourcecommitstart = 0xf71C,
    sourcecommitstartnextlocal = 0xf71D,
    sourcefork = 0xf4C1,
    sourcemerge = 0xf62D,
    sourcepull = 0xf4C2,
    speaker = 0xf4C3,
    speakeroff = 0xf4C4,
    speakerwireless = 0xf71E,
    speedometer = 0xf4C5,
    spellcheck = 0xf4C6,
    spotify = 0xf4C7,
    spotlight = 0xf4C8,
    spotlightbeam = 0xf4C9,
    spray = 0xf665,
    squareinc = 0xf4CA,
    squareinccash = 0xf4CB,
    stackexchange = 0xf60B,
    stackoverflow = 0xf4CC,
    stadium = 0xf71F,
    stairs = 0xf4CD,
    star = 0xf4CE,
    starcircle = 0xf4CF,
    starhalf = 0xf4D0,
    staroff = 0xf4D1,
    staroutline = 0xf4D2,
    steam = 0xf4D3,
    steering = 0xf4D4,
    stepbackward = 0xf4D5,
    stepbackward2 = 0xf4D6,
    stepforward = 0xf4D7,
    stepforward2 = 0xf4D8,
    stethoscope = 0xf4D9,
    sticker = 0xf5D0,
    stocking = 0xf4DA,
    stop = 0xf4DB,
    stopcircle = 0xf666,
    stopcircleoutline = 0xf667,
    store = 0xf4DC,
    store24hour = 0xf4DD,
    stove = 0xf4DE,
    subdirectoryarrowleft = 0xf60C,
    subdirectoryarrowright = 0xf60D,
    subway = 0xf6AB,
    subwayvariant = 0xf4DF,
    sunglasses = 0xf4E0,
    surroundsound = 0xf5C5,
    svg = 0xf720,
    swaphorizontal = 0xf4E1,
    swapvertical = 0xf4E2,
    swim = 0xf4E3,
    switch_ = 0xf4E4,
    sword = 0xf4E5,
    sync = 0xf4E6,
    syncalert = 0xf4E7,
    syncoff = 0xf4E8,
    tab = 0xf4E9,
    tabunselected = 0xf4EA,
    table = 0xf4EB,
    tablecolumnplusafter = 0xf4EC,
    tablecolumnplusbefore = 0xf4ED,
    tablecolumnremove = 0xf4EE,
    tablecolumnwidth = 0xf4EF,
    tableedit = 0xf4F0,
    tablelarge = 0xf4F1,
    tablerowheight = 0xf4F2,
    tablerowplusafter = 0xf4F3,
    tablerowplusbefore = 0xf4F4,
    tablerowremove = 0xf4F5,
    tablet = 0xf4F6,
    tabletandroid = 0xf4F7,
    tabletipad = 0xf4F8,
    tag = 0xf4F9,
    tagfaces = 0xf4FA,
    tagheart = 0xf68A,
    tagmultiple = 0xf4FB,
    tagoutline = 0xf4FC,
    tagplus = 0xf721,
    tagremove = 0xf722,
    tagtextoutline = 0xf4FD,
    target = 0xf4FE,
    taxi = 0xf4FF,
    teamviewer = 0xf500,
    telegram = 0xf501,
    television = 0xf502,
    televisionguide = 0xf503,
    temperaturecelsius = 0xf504,
    temperaturefahrenheit = 0xf505,
    temperaturekelvin = 0xf506,
    tennis = 0xf507,
    tent = 0xf508,
    terrain = 0xf509,
    testtube = 0xf668,
    textshadow = 0xf669,
    texttospeech = 0xf50A,
    texttospeechoff = 0xf50B,
    textbox = 0xf60E,
    texture = 0xf50C,
    theater = 0xf50D,
    themelightdark = 0xf50E,
    thermometer = 0xf50F,
    thermometerlines = 0xf510,
    thumbdown = 0xf511,
    thumbdownoutline = 0xf512,
    thumbup = 0xf513,
    thumbupoutline = 0xf514,
    thumbsupdown = 0xf515,
    ticket = 0xf516,
    ticketaccount = 0xf517,
    ticketconfirmation = 0xf518,
    ticketpercent = 0xf723,
    tie = 0xf519,
    tilde = 0xf724,
    timelapse = 0xf51A,
    timer = 0xf51B,
    timer10 = 0xf51C,
    timer3 = 0xf51D,
    timeroff = 0xf51E,
    timersand = 0xf51F,
    timersandempty = 0xf6AC,
    timetable = 0xf520,
    toggleswitch = 0xf521,
    toggleswitchoff = 0xf522,
    tooltip = 0xf523,
    tooltipedit = 0xf524,
    tooltipimage = 0xf525,
    tooltipoutline = 0xf526,
    tooltipoutlineplus = 0xf527,
    tooltiptext = 0xf528,
    tooth = 0xf529,
    tor = 0xf52A,
    towerbeach = 0xf680,
    towerfire = 0xf681,
    trafficlight = 0xf52B,
    train = 0xf52C,
    tram = 0xf52D,
    transcribe = 0xf52E,
    transcribeclose = 0xf52F,
    transfer = 0xf530,
    transittransfer = 0xf6AD,
    translate = 0xf5CA,
    treasurechest = 0xf725,
    tree = 0xf531,
    trello = 0xf532,
    trendingdown = 0xf533,
    trendingneutral = 0xf534,
    trendingup = 0xf535,
    triangle = 0xf536,
    triangleoutline = 0xf537,
    trophy = 0xf538,
    trophyaward = 0xf539,
    trophyoutline = 0xf53A,
    trophyvariant = 0xf53B,
    trophyvariantoutline = 0xf53C,
    truck = 0xf53D,
    truckdelivery = 0xf53E,
    trucktrailer = 0xf726,
    tshirtcrew = 0xf53F,
    tshirtv = 0xf540,
    tumblr = 0xf541,
    tumblrreblog = 0xf542,
    tune = 0xf62E,
    tunevertical = 0xf66A,
    twitch = 0xf543,
    twitter = 0xf544,
    twitterbox = 0xf545,
    twittercircle = 0xf546,
    twitterretweet = 0xf547,
    ubuntu = 0xf548,
    umbraco = 0xf549,
    umbrella = 0xf54A,
    umbrellaoutline = 0xf54B,
    undo = 0xf54C,
    undovariant = 0xf54D,
    unfoldless = 0xf54E,
    unfoldmore = 0xf54F,
    ungroup = 0xf550,
    unity = 0xf6AE,
    untappd = 0xf551,
    update = 0xf6AF,
    upload = 0xf552,
    usb = 0xf553,
    vectorarrangeabove = 0xf554,
    vectorarrangebelow = 0xf555,
    vectorcircle = 0xf556,
    vectorcirclevariant = 0xf557,
    vectorcombine = 0xf558,
    vectorcurve = 0xf559,
    vectordifference = 0xf55A,
    vectordifferenceab = 0xf55B,
    vectordifferenceba = 0xf55C,
    vectorintersection = 0xf55D,
    vectorline = 0xf55E,
    vectorpoint = 0xf55F,
    vectorpolygon = 0xf560,
    vectorpolyline = 0xf561,
    vectorrectangle = 0xf5C6,
    vectorselection = 0xf562,
    vectorsquare = 0xf001,
    vectortriangle = 0xf563,
    vectorunion = 0xf564,
    verified = 0xf565,
    vibrate = 0xf566,
    video = 0xf567,
    videooff = 0xf568,
    videoswitch = 0xf569,
    viewagenda = 0xf56A,
    viewarray = 0xf56B,
    viewcarousel = 0xf56C,
    viewcolumn = 0xf56D,
    viewdashboard = 0xf56E,
    viewday = 0xf56F,
    viewgrid = 0xf570,
    viewheadline = 0xf571,
    viewlist = 0xf572,
    viewmodule = 0xf573,
    viewparallel = 0xf727,
    viewquilt = 0xf574,
    viewsequential = 0xf728,
    viewstream = 0xf575,
    viewweek = 0xf576,
    vimeo = 0xf577,
    vine = 0xf578,
    violin = 0xf60F,
    visualstudio = 0xf610,
    vk = 0xf579,
    vkbox = 0xf57A,
    vkcircle = 0xf57B,
    vlc = 0xf57C,
    voice = 0xf5CB,
    voicemail = 0xf57D,
    volumehigh = 0xf57E,
    volumelow = 0xf57F,
    volumemedium = 0xf580,
    volumeoff = 0xf581,
    vpn = 0xf582,
    walk = 0xf583,
    wallet = 0xf584,
    walletgiftcard = 0xf585,
    walletmembership = 0xf586,
    wallettravel = 0xf587,
    wan = 0xf588,
    washingmachine = 0xf729,
    watch = 0xf589,
    watchexport = 0xf58A,
    watchimport = 0xf58B,
    watchvibrate = 0xf6B0,
    water = 0xf58C,
    wateroff = 0xf58D,
    waterpercent = 0xf58E,
    waterpump = 0xf58F,
    watermark = 0xf612,
    weathercloudy = 0xf590,
    weatherfog = 0xf591,
    weatherhail = 0xf592,
    weatherlightning = 0xf593,
    weatherlightningrainy = 0xf67D,
    weathernight = 0xf594,
    weatherpartlycloudy = 0xf595,
    weatherpouring = 0xf596,
    weatherrainy = 0xf597,
    weathersnowy = 0xf598,
    weathersnowyrainy = 0xf67E,
    weathersunny = 0xf599,
    weathersunset = 0xf59A,
    weathersunsetdown = 0xf59B,
    weathersunsetup = 0xf59C,
    weatherwindy = 0xf59D,
    weatherwindyvariant = 0xf59E,
    web = 0xf59F,
    webcam = 0xf5A0,
    webhook = 0xf62F,
    webpack = 0xf72A,
    wechat = 0xf611,
    weight = 0xf5A1,
    weightkilogram = 0xf5A2,
    whatsapp = 0xf5A3,
    wheelchairaccessibility = 0xf5A4,
    whitebalanceauto = 0xf5A5,
    whitebalanceincandescent = 0xf5A6,
    whitebalanceiridescent = 0xf5A7,
    whitebalancesunny = 0xf5A8,
    widgets = 0xf72B,
    wifi = 0xf5A9,
    wifioff = 0xf5AA,
    wii = 0xf5AB,
    wiiu = 0xf72C,
    wikipedia = 0xf5AC,
    windowclose = 0xf5AD,
    windowclosed = 0xf5AE,
    windowmaximize = 0xf5AF,
    windowminimize = 0xf5B0,
    windowopen = 0xf5B1,
    windowrestore = 0xf5B2,
    windows = 0xf5B3,
    wordpress = 0xf5B4,
    worker = 0xf5B5,
    wrap = 0xf5B6,
    wrench = 0xf5B7,
    wunderlist = 0xf5B8,
    xaml = 0xf673,
    xbox = 0xf5B9,
    xboxcontroller = 0xf5BA,
    xboxcontrolleroff = 0xf5BB,
    xda = 0xf5BC,
    xing = 0xf5BD,
    xingbox = 0xf5BE,
    xingcircle = 0xf5BF,
    xml = 0xf5C0,
    yeast = 0xf5C1,
    yelp = 0xf5C2,
    yinyang = 0xf67F,
    youtubeplay = 0xf5C3,
    zipbox = 0xf5C4

  };
};
}

#endif // TULIPMATERIALDESIGNICONS_H
