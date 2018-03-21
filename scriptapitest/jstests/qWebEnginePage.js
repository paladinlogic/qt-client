/**
* Test the QWebEnginePage object
*/
var tmp = new QWebEnginePage()
    , properties = [
        'audioMuted'
        , 'backgroundColor'
        , 'contentsSize'
        , 'hasSelection'
        , 'icon'
        , 'iconUrl'
        , 'recentlyAudible'
        , 'requestedUrl'
        , 'scrollPosition'
        , 'selectedText'
        , 'title'
        , 'url'
        , 'zoomFactor'
    ]
    , publicFunctions = [
        'action'
        , 'contextMenuData'
        , 'createStandardContextMenu'
        , 'download'
        , 'findText'
        , 'history'
        , 'isAudioMuted'
        , 'load'
        , 'print'
        , 'printToPdf'
        , 'profile'
        , 'replaceMisspelledWord'
        , 'runJavaScript'
        , 'save'
        , 'scripts'
        , 'setAudioMuted'
        , 'setBackgroundColor'
        , 'setContent'
        , 'setFeaturePermission'
        , 'setHtml'
        , 'setUrl'
        , 'setView'
        , 'setWebChannel'
        , 'setZoomFactor'
        , 'settings'
        , 'toHtml'
        , 'triggerAction'
        , 'view'
        , 'webChannel'
    ];

// Test constructor
assertIsConstructor(QWebEnginePage, 'QWebEnginePage');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QWebEnginePage', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
