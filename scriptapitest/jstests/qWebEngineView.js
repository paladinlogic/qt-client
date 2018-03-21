/**
* Test the QWebEngineView object
*/
var tmp = new QWebEngineView()
    , properties = [
        'hasSelection'
        , 'icon'
        , 'iconUrl'
        , 'selectedText'
        , 'title'
        , 'url'
        , 'zoomFactor'
    ]
    , publicFunctions = [
        'findText'
        , 'history'
        , 'load'
        , 'page'
        , 'pageAction'
        , 'setContent'
        , 'setHtml'
        , 'setPage'
        , 'setUrl'
        , 'setZoomFactor'
        , 'settings'
    ];

assertIsConstructor(QWebEngineView, 'QWebEngineView');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QWebEngineView', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
