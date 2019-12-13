// MStar devices

#ifndef QT_NO_DBUS
#  define QT_NO_DBUS
#endif

/* Data structures */
#ifndef QT_NO_QUUID_STRING
#  define QT_NO_QUUID_STRING
#endif

/* Dialogs */
#ifndef QT_NO_COLORDIALOG
#  define QT_NO_COLORDIALOG
#endif
#ifndef QT_NO_ERRORMESSAGE
#  define QT_NO_ERRORMESSAGE
#endif
#ifndef QT_NO_FILEDIALOG
#  define QT_NO_FILEDIALOG
#endif
#ifndef QT_NO_FONTDIALOG
#  define QT_NO_FONTDIALOG
#endif
#ifndef QT_NO_PRINTDIALOG
#  define QT_NO_PRINTDIALOG
#endif
#ifndef QT_NO_PRINTPREVIEWDIALOG
#  define QT_NO_PRINTPREVIEWDIALOG
#endif
#ifndef QT_NO_PROGRESSDIALOG
#  define QT_NO_PROGRESSDIALOG
#endif
#ifndef QT_NO_TABDIALOG
#  define QT_NO_TABDIALOG
#endif
#ifndef QT_NO_WIZARD
#  define QT_NO_WIZARD
#endif

/* File I/O */
#ifndef QT_NO_DOM
#  define QT_NO_DOM
#endif
#ifndef QT_NO_FILESYSTEMWATCHER
#  define QT_NO_FILESYSTEMWATCHER
#endif
#ifndef QT_NO_FILESYSTEMMODEL
#  define QT_NO_FILESYSTEMMODEL
#endif
#ifndef QT_NO_PROCESS
#  define QT_NO_PROCESS
#endif
#ifndef QT_NO_TEMPORARYFILE
#  define QT_NO_TEMPORARYFILE
#endif
#ifndef QT_NO_TEXTSTREAM
//#  define QT_NO_TEXTSTREAM //used in QtOpenGL by qWarning() << 
#endif

/* Fonts */
#ifndef QT_NO_QWS_QPF
#  define QT_NO_QWS_QPF
#endif
#ifndef QT_NO_QWS_QPF2
#  define QT_NO_QWS_QPF2
#endif

/* Images */
#ifndef QT_NO_IMAGEFORMATPLUGIN
#  define QT_NO_IMAGEFORMATPLUGIN
#endif
#ifndef QT_NO_IMAGEFORMAT_XBM
#  define QT_NO_IMAGEFORMAT_XBM
#endif
#ifndef QT_NO_IMAGEFORMAT_XPM
#  define QT_NO_IMAGEFORMAT_XPM
#endif
#ifndef QT_NO_IMAGE_HEURISTIC_MASK
#  define QT_NO_IMAGE_HEURISTIC_MASK
#endif
#ifndef QT_NO_IMAGE_TEXT
#  define QT_NO_IMAGE_TEXT
#endif

/* Internationalization */
#ifndef QT_NO_QWS_INPUTMETHODS
#  define QT_NO_QWS_INPUTMETHODS
#endif

/* ItemViews */
#ifndef QT_NO_DATAWIDGETMAPPER
#  define QT_NO_DATAWIDGETMAPPER
#endif
#ifndef QT_NO_DIRMODEL
#  define QT_NO_DIRMODEL
#endif
#ifndef QT_NO_COLUMNVIEW
#  define QT_NO_COLUMNVIEW
#endif
#ifndef QT_NO_SORTFILTERPROXYMODEL
#  define QT_NO_SORTFILTERPROXYMODEL
#endif
#ifndef QT_NO_STANDARDITEMMODEL
//#  define QT_NO_STANDARDITEMMODEL //needed by QComboBox <- QInputDialog
#endif
#ifndef QT_NO_TABLEVIEW
//#  define QT_NO_TABLEVIEW //needed by TABLEWIDGET, which needed by OpenBrowser
#endif

/* Kernel */
#ifndef QT_NO_SESSIONMANAGER
#  define QT_NO_SESSIONMANAGER
#endif
#ifndef QT_NO_DRAGANDDROP
#  define QT_NO_DRAGANDDROP
#endif
#ifndef QT_NO_EFFECTS
#  define QT_NO_EFFECTS
#endif
#ifndef QT_NO_SHAREDMEMORY
//#  define QT_NO_SHAREDMEMORY
#endif
#ifndef QT_NO_SHORTCUT
//#  define QT_NO_SHORTCUT //needed by QMenuBar
#endif
#ifndef QT_NO_SOUND
#  define QT_NO_SOUND
#endif
#ifndef QT_NO_SYSTEMSEMAPHORE
#  define QT_NO_SYSTEMSEMAPHORE
#endif
#ifndef QT_NO_TABLETEVENT
#  define QT_NO_TABLETEVENT
#endif
#ifndef QT_NO_TEXTHTMLPARSER
#  define QT_NO_TEXTHTMLPARSER
#endif
#ifndef QT_NO_WHEELEVENT
//#  define QT_NO_WHEELEVENT //needed by WebKit/qt/WebCoreSupport/WebEventConversion
#endif
#ifndef QT_NO_XMLSTREAM
//#  define QT_NO_XMLSTREAM //needed by WebCore/xml/parser
#endif
#ifndef QT_NO_XMLSTREAMREADER
//#  define QT_NO_XMLSTREAMREADER //needed by WebCore/xml/parser
#endif
#ifndef QT_NO_XMLSTREAMWRITER
#  define QT_NO_XMLSTREAMWRITER
#endif

/* Networking */
#ifndef QT_NO_COP
#  define QT_NO_COP
#endif
#ifndef QT_NO_URLINFO
#  define QT_NO_URLINFO
#endif
#ifndef QT_NO_FTP
#  define QT_NO_FTP
#endif

/* Painting */
#ifndef QT_NO_DIRECTPAINTER
#  define QT_NO_DIRECTPAINTER
#endif
#ifndef QT_NO_PAINT_DEBUG
#  define QT_NO_PAINT_DEBUG
#endif
#ifndef QT_NO_PICTURE
#  define QT_NO_PICTURE
#endif
#ifndef QT_NO_PRINTER
#  define QT_NO_PRINTER
#endif
#ifndef QT_NO_CUPS
#  define QT_NO_CUPS
#endif

/* Phonon */
#ifndef QT_NO_PHONON_ABSTRACTMEDIASTREAM
#  define QT_NO_PHONON_ABSTRACTMEDIASTREAM
#endif
#ifndef QT_NO_PHONON_AUDIOCAPTURE
#  define QT_NO_PHONON_AUDIOCAPTURE
#endif
#ifndef QT_NO_PHONON_EFFECT
#  define QT_NO_PHONON_EFFECT
#endif
#ifndef QT_NO_PHONON_MEDIACONTROLLER
#  define QT_NO_PHONON_MEDIACONTROLLER
#endif
#ifndef QT_NO_PHONON_OBJECTDESCRIPTIONMODEL
#  define QT_NO_PHONON_OBJECTDESCRIPTIONMODEL
#endif
#ifndef QT_NO_PHONON_PLATFORMPLUGIN
#  define QT_NO_PHONON_PLATFORMPLUGIN
#endif
#ifndef QT_NO_PHONON_SEEKSLIDER
#  define QT_NO_PHONON_SEEKSLIDER
#endif
#ifndef QT_NO_PHONON_SETTINGSGROUP
#  define QT_NO_PHONON_SETTINGSGROUP
#endif
#ifndef QT_NO_PHONON_VIDEO
#  define QT_NO_PHONON_VIDEO
#endif
#ifndef QT_NO_PHONON_VOLUMESLIDER
#  define QT_NO_PHONON_VOLUMESLIDER
#endif

/* Qt for Embedded Linux */
#ifndef QT_NO_QWS_DECORATION_DEFAULT
#  define QT_NO_QWS_DECORATION_DEFAULT
#endif
#ifndef QT_NO_QWS_DECORATION_STYLED
#  define QT_NO_QWS_DECORATION_STYLED
#endif
#ifndef QT_NO_QWS_DECORATION_WINDOWS
#  define QT_NO_QWS_DECORATION_WINDOWS
#endif
#ifndef QT_NO_QWS_SOUNDSERVER
#  define QT_NO_QWS_SOUNDSERVER
#endif
#ifndef QT_NO_SXE
#  define QT_NO_SXE
#endif

/* SVG */
#ifndef QT_NO_SVG
#  define QT_NO_SVG
#endif
#ifndef QT_NO_SVGGENERATOR
#  define QT_NO_SVGGENERATOR
#endif
#ifndef QT_NO_SVGRENDERER
#  define QT_NO_SVGRENDERER
#endif
#ifndef QT_NO_SVGWIDGET
#  define QT_NO_SVGWIDGET
#endif

/* Styles */
#ifndef QT_NO_STYLE_MOTIF
#  define QT_NO_STYLE_MOTIF
#endif
#ifndef QT_NO_STYLE_CDE
#  define QT_NO_STYLE_CDE
#endif
#ifndef QT_NO_STYLE_CLEANLOOKS
#  define QT_NO_STYLE_CLEANLOOKS
#endif
#ifndef QT_NO_STYLE_WINDOWSCE
#  define QT_NO_STYLE_WINDOWSCE
#endif
#ifndef QT_NO_STYLE_WINDOWSMOBILE
#  define QT_NO_STYLE_WINDOWSMOBILE
#endif
#ifndef QT_NO_STYLE_WINDOWSVISTA
#  define QT_NO_STYLE_WINDOWSVISTA
#endif
#ifndef QT_NO_STYLE_WINDOWSXP
#  define QT_NO_STYLE_WINDOWSXP
#endif

/* Utilities */
#ifndef QT_NO_ACCESSIBILITY
#  define QT_NO_ACCESSIBILITY
#endif
#ifndef QT_NO_DESKTOPSERVICES
#  define QT_NO_DESKTOPSERVICES
#endif
#ifndef QT_NO_SYSTEMTRAYICON
#  define QT_NO_SYSTEMTRAYICON
#endif
#ifndef QT_NO_UNDOGROUP
#  define QT_NO_UNDOGROUP
#endif
#ifndef QT_NO_UNDOSTACK
#  define QT_NO_UNDOSTACK
#endif
#ifndef QT_NO_UNDOVIEW
#  define QT_NO_UNDOVIEW
#endif
#ifndef QT_NO_GESTURES
#  define QT_NO_GESTURES
#endif

/* Widgets */
#ifndef QT_NO_LCDNUMBER
#  define QT_NO_LCDNUMBER
#endif
#ifndef QT_NO_FONTCOMBOBOX
#  define QT_NO_FONTCOMBOBOX
#endif
#ifndef QT_NO_SPINBOX
//#  define QT_NO_SPINBOX //needed by QInputDialog <- QtTestBrowser/MStarWebBrowser locationedit.cpp
#endif
#ifndef QT_NO_CALENDARWIDGET
#  define QT_NO_CALENDARWIDGET
#endif
#ifndef QT_NO_DATETIMEEDIT
#  define QT_NO_DATETIMEEDIT
#endif
#ifndef QT_NO_CONTEXTMENU
//#  define QT_NO_CONTEXTMENU //needed by QtTestBrowser/MStarWebBrowser webview.cpp 
#endif
#ifndef QT_NO_DOCKWIDGET
//#  define QT_NO_DOCKWIDGET    //needed by OpenBrowser
#endif
#ifndef QT_NO_PRINTERPREVIEWWIDGET
#  define QT_NO_PERINTERPREVIEWWIDGET
#endif
#ifndef QT_NO_WORKSPACE
#  define QT_NO_WORKSPACE
#endif
#ifndef QT_NO_RESIZEHANDLER
//#  define QT_NO_RESIZEHANDLER //needed by QMainWindow
#endif
#ifndef QT_NO_SIZEGRIP
#  define QT_NO_SIZEGRIP
#endif
#ifndef QT_NO_DIAL
#  define QT_NO_DIAL
#endif
#ifndef QT_NO_PRINTPREVIEWWIDGET
#  define QT_NO_PRINTPREVIEWWIDGET
#endif
#ifndef QT_NO_TEXTEDIT
//#  define QT_NO_TEXTEDIT //needed by TEXTBROWSER, which needed by OpenBrowser
#endif
#ifndef QT_NO_SYNTAXHIGHLIGHTER
#  define QT_NO_SYNTAXHIGHLIGHTER
#endif
#ifndef QT_NO_TEXTBROWSER
//#  define QT_NO_TEXTBROWSER //needed by OpenBrowser
#endif
#ifndef QT_NO_SPINWIDGET
//#  define QT_NO_SPINWIDGET  // needed by QSpinBox <- QInputDialog
#endif
#ifndef QT_NO_SPLASHSCREEN
#  define QT_NO_SPLASHSCREEN
#endif
#ifndef QT_NO_STATUSTIP
#  define QT_NO_STATUSTIP
#endif
#ifndef QT_NO_TOOLBOX
#  define QT_NO_TOOLBOX
#endif
#ifndef QT_NO_TREEWIDGET
#  define QT_NO_TREEWIDGET
#endif
#ifndef QT_NO_VALIDATOR
//#  define QT_NO_VALIDATOR  // needed by QSpinBox <- QInputDialog
#endif

/* Windows */
#ifndef QT_NO_WIN_ACTIVEQT
#  define QT_NO_WIN_ACTIVEQT
#endif

#ifndef QT_NO_IPV6
#define QT_NO_IPV6
#endif

#ifndef QT_NO_IPV6IFNAME
#define QT_NO_IPV6IFNAME
#endif

/* MStar-specific */
#define MSTAR_REMOTE
