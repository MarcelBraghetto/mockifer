if (typeof kotlin === 'undefined') {
  throw new Error("Error loading module 'content_main'. Its dependency 'kotlin' was not found. Please, check whether 'kotlin' is loaded prior to 'content_main'.");
}
var content_main = function (_, Kotlin) {
  'use strict';
  var firstOrNull = Kotlin.kotlin.collections.firstOrNull_2p1efm$;
  var Pair = Kotlin.kotlin.Pair;
  var toMutableList = Kotlin.kotlin.collections.toMutableList_us0mfu$;
  var lazy = Kotlin.kotlin.lazy_klfg04$;
  var startsWith = Kotlin.kotlin.text.startsWith_7epoxm$;
  var lastIndexOf = Kotlin.kotlin.text.lastIndexOf_l5u8uk$;
  var toIntOrNull = Kotlin.kotlin.text.toIntOrNull_pdl1vz$;
  var split = Kotlin.kotlin.text.split_ip8yn$;
  var toMap = Kotlin.kotlin.collections.toMap_abgq59$;
  CatsApi$GetAllCatsController$respondToRequest$ObjectLiteral.prototype = Object.create(ResponseBody.prototype);
  CatsApi$GetAllCatsController$respondToRequest$ObjectLiteral.prototype.constructor = CatsApi$GetAllCatsController$respondToRequest$ObjectLiteral;
  CatsApi$GetAllCatsController.prototype = Object.create(RequestController.prototype);
  CatsApi$GetAllCatsController.prototype.constructor = CatsApi$GetAllCatsController;
  CatsApi$CatDetailsController$respondToRequest$ObjectLiteral.prototype = Object.create(ResponseBody.prototype);
  CatsApi$CatDetailsController$respondToRequest$ObjectLiteral.prototype.constructor = CatsApi$CatDetailsController$respondToRequest$ObjectLiteral;
  CatsApi$CatDetailsController.prototype = Object.create(RequestController.prototype);
  CatsApi$CatDetailsController.prototype.constructor = CatsApi$CatDetailsController;
  CatsApi$CreateCatController$respondToRequest$ObjectLiteral.prototype = Object.create(ResponseBody.prototype);
  CatsApi$CreateCatController$respondToRequest$ObjectLiteral.prototype.constructor = CatsApi$CreateCatController$respondToRequest$ObjectLiteral;
  CatsApi$CreateCatController.prototype = Object.create(RequestController.prototype);
  CatsApi$CreateCatController.prototype.constructor = CatsApi$CreateCatController;
  CatsApi$DeleteCatController.prototype = Object.create(RequestController.prototype);
  CatsApi$DeleteCatController.prototype.constructor = CatsApi$DeleteCatController;
  CatsApi$UpdateCatController$respondToRequest$ObjectLiteral.prototype = Object.create(ResponseBody.prototype);
  CatsApi$UpdateCatController$respondToRequest$ObjectLiteral.prototype.constructor = CatsApi$UpdateCatController$respondToRequest$ObjectLiteral;
  CatsApi$UpdateCatController.prototype = Object.create(RequestController.prototype);
  CatsApi$UpdateCatController.prototype.constructor = CatsApi$UpdateCatController;
  MockiferApi$JsonFileMockResponseController.prototype = Object.create(RequestController.prototype);
  MockiferApi$JsonFileMockResponseController.prototype.constructor = MockiferApi$JsonFileMockResponseController;
  MockiferApi$HtmlExampleController.prototype = Object.create(RequestController.prototype);
  MockiferApi$HtmlExampleController.prototype.constructor = MockiferApi$HtmlExampleController;
  MockiferApi$GetAllRoutesController.prototype = Object.create(RequestController.prototype);
  MockiferApi$GetAllRoutesController.prototype.constructor = MockiferApi$GetAllRoutesController;
  MockiferApi$GetAllActiveMocks.prototype = Object.create(RequestController.prototype);
  MockiferApi$GetAllActiveMocks.prototype.constructor = MockiferApi$GetAllActiveMocks;
  ErrorResponseBody.prototype = Object.create(ResponseBody.prototype);
  ErrorResponseBody.prototype.constructor = ErrorResponseBody;
  function Cat(id, name, age, image) {
    Cat$Companion_getInstance();
    this.id = id;
    this.name = name;
    this.age = age;
    this.image = image;
  }
  function Cat$Companion() {
    Cat$Companion_instance = this;
  }
  Cat$Companion.prototype.revive_11rb$ = function (dto) {
    dto.id = orDefault(dto.id, '');
    dto.name = orDefault(dto.name, '');
    dto.age = orDefault(dto.age, 0);
    dto.image = orDefault(dto.image, '');
  };
  Cat$Companion.prototype.isValid_11rb$ = function (dto) {
    return dto.name.length > 0;
  };
  Cat$Companion.$metadata$ = {kind: Kotlin.Kind.OBJECT, simpleName: 'Companion', interfaces: [DtoCompanion]};
  var Cat$Companion_instance = null;
  function Cat$Companion_getInstance() {
    if (Cat$Companion_instance === null) {
      new Cat$Companion();
    }
    return Cat$Companion_instance;
  }
  function CatsApi() {
    CatsApi_instance = this;
  }
  function CatsApi$GetAllCatsController(serverSession) {
    RequestController.call(this, serverSession);
  }
  function CatsApi$GetAllCatsController$respondToRequest$ObjectLiteral(this$GetAllCatsController, serverSession_0) {
    ResponseBody.call(this, serverSession_0);
    this.cats = this$GetAllCatsController.serverSession.catsRepository.getAllCats();
  }
  CatsApi$GetAllCatsController$respondToRequest$ObjectLiteral.$metadata$ = {kind: Kotlin.Kind.CLASS, interfaces: [ResponseBody]};
  CatsApi$GetAllCatsController.prototype.respondToRequest_38q16g$ = function (route, request) {
    return new Response(StatusCode_getInstance().SUCCESS, this.createResponseHeaders_xj066j$(request), toJson(new CatsApi$GetAllCatsController$respondToRequest$ObjectLiteral(this, this.serverSession)));
  };
  CatsApi$GetAllCatsController.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'GetAllCatsController', interfaces: [RequestController]};
  function CatsApi$CatDetailsController(serverSession) {
    RequestController.call(this, serverSession);
  }
  function CatsApi$CatDetailsController$respondToRequest$ObjectLiteral(closure$cat, serverSession_0) {
    ResponseBody.call(this, serverSession_0);
    this.cat = closure$cat;
  }
  CatsApi$CatDetailsController$respondToRequest$ObjectLiteral.$metadata$ = {kind: Kotlin.Kind.CLASS, interfaces: [ResponseBody]};
  CatsApi$CatDetailsController.prototype.respondToRequest_38q16g$ = function (route, request) {
    var tmp$;
    var responseHeaders = this.createResponseHeaders_xj066j$(request);
    var $receiver = route.requestUri;
    var id = getFirstMatch(Kotlin.kotlin.text.Regex_61zpoe$($receiver), request.uri);
    tmp$ = this.serverSession.catsRepository.getCat_61zpoe$(id);
    if (tmp$ == null) {
      return new Response(StatusCode_getInstance().NOT_FOUND_ERROR, responseHeaders, toJson(new ErrorResponseBody('Cat with id ' + id + ' not found.', this.serverSession)));
    }
    var cat = tmp$;
    return new Response(StatusCode_getInstance().SUCCESS, responseHeaders, toJson(new CatsApi$CatDetailsController$respondToRequest$ObjectLiteral(cat, this.serverSession)));
  };
  CatsApi$CatDetailsController.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'CatDetailsController', interfaces: [RequestController]};
  function CatsApi$CreateCatController(serverSession) {
    RequestController.call(this, serverSession);
  }
  function CatsApi$CreateCatController$respondToRequest$ObjectLiteral(this$CreateCatController, closure$cat, serverSession_0) {
    ResponseBody.call(this, serverSession_0);
    this.cat = this$CreateCatController.serverSession.catsRepository.createCat_hu72qb$(closure$cat);
  }
  CatsApi$CreateCatController$respondToRequest$ObjectLiteral.$metadata$ = {kind: Kotlin.Kind.CLASS, interfaces: [ResponseBody]};
  CatsApi$CreateCatController.prototype.respondToRequest_38q16g$ = function (route, request) {
    var tmp$;
    var responseHeaders = this.createResponseHeaders_xj066j$(request);
    tmp$ = Cat$Companion_getInstance().fromJson_pdl1vj$(request.body);
    if (tmp$ == null) {
      return new Response(StatusCode_getInstance().BAD_REQUEST, responseHeaders, toJson(new ErrorResponseBody('Unable to parse request body: ' + request.body, this.serverSession)));
    }
    var cat = tmp$;
    return new Response(StatusCode_getInstance().SUCCESS, responseHeaders, toJson(new CatsApi$CreateCatController$respondToRequest$ObjectLiteral(this, cat, this.serverSession)));
  };
  CatsApi$CreateCatController.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'CreateCatController', interfaces: [RequestController]};
  function CatsApi$DeleteCatController(serverSession) {
    RequestController.call(this, serverSession);
  }
  CatsApi$DeleteCatController.prototype.respondToRequest_38q16g$ = function (route, request) {
    var responseHeaders = this.createResponseHeaders_xj066j$(request);
    var $receiver = route.requestUri;
    var id = getFirstMatch(Kotlin.kotlin.text.Regex_61zpoe$($receiver), request.uri);
    if (this.serverSession.catsRepository.deleteCat_61zpoe$(id)) {
      return new Response(StatusCode_getInstance().SUCCESS, responseHeaders, toJson(new ResponseBody(this.serverSession)));
    }
    return new Response(StatusCode_getInstance().NOT_FOUND_ERROR, responseHeaders, toJson(new ErrorResponseBody('Cat with id ' + id + ' not found.', this.serverSession)));
  };
  CatsApi$DeleteCatController.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'DeleteCatController', interfaces: [RequestController]};
  function CatsApi$UpdateCatController(serverSession) {
    RequestController.call(this, serverSession);
  }
  function CatsApi$UpdateCatController$respondToRequest$ObjectLiteral(closure$result, serverSession_0) {
    ResponseBody.call(this, serverSession_0);
    this.originalCat = closure$result.first;
    this.newCat = closure$result.second;
  }
  CatsApi$UpdateCatController$respondToRequest$ObjectLiteral.$metadata$ = {kind: Kotlin.Kind.CLASS, interfaces: [ResponseBody]};
  CatsApi$UpdateCatController.prototype.respondToRequest_38q16g$ = function (route, request) {
    var tmp$, tmp$_0;
    var responseHeaders = this.createResponseHeaders_xj066j$(request);
    tmp$ = Cat$Companion_getInstance().fromJson_pdl1vj$(request.body);
    if (tmp$ == null) {
      return new Response(StatusCode_getInstance().BAD_REQUEST, responseHeaders, toJson(new ErrorResponseBody('Cannot update cat - unable to parse json: ' + request.body, this.serverSession)));
    }
    var cat = tmp$;
    tmp$_0 = this.serverSession.catsRepository.updateCat_hu72qb$(cat);
    if (tmp$_0 == null) {
      return new Response(StatusCode_getInstance().NOT_FOUND_ERROR, responseHeaders, toJson(new ErrorResponseBody('Cat to update was not found.', this.serverSession)));
    }
    var result = tmp$_0;
    return new Response(StatusCode_getInstance().SUCCESS, responseHeaders, toJson(new CatsApi$UpdateCatController$respondToRequest$ObjectLiteral(result, this.serverSession)));
  };
  CatsApi$UpdateCatController.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'UpdateCatController', interfaces: [RequestController]};
  var CatsApi_instance = null;
  function CatsRepository() {
    this.cats$delegate = lazy(Kotlin.getCallableRef('loadCats', function ($receiver) {
      return $receiver.loadCats_0();
    }.bind(null, this)));
  }
  Object.defineProperty(CatsRepository.prototype, 'cats_0', {get: function () {
    var $receiver = this.cats$delegate;
    new Kotlin.PropertyMetadata('cats');
    return $receiver.value;
  }});
  CatsRepository.prototype.getAllCats = function () {
    return this.cats_0;
  };
  CatsRepository.prototype.getCat_61zpoe$ = function (id) {
    var $receiver = this.cats_0;
    var destination = Kotlin.kotlin.collections.ArrayList_init_ww73n8$();
    var tmp$;
    tmp$ = $receiver.iterator();
    while (tmp$.hasNext()) {
      var element = tmp$.next();
      if (Kotlin.equals(element.id, id))
        destination.add_11rb$(element);
    }
    return firstOrNull(destination);
  };
  CatsRepository.prototype.deleteCat_61zpoe$ = function (id) {
    var $receiver = this.cats_0;
    var indexOfFirst$result;
    indexOfFirst$break: do {
      var tmp$;
      var index = 0;
      tmp$ = $receiver.iterator();
      while (tmp$.hasNext()) {
        var item = tmp$.next();
        if (Kotlin.equals(item.id, id)) {
          indexOfFirst$result = index;
          break indexOfFirst$break;
        }
        index = index + 1 | 0;
      }
      indexOfFirst$result = -1;
    }
     while (false);
    var index_0 = indexOfFirst$result;
    if (index_0 >= 0) {
      this.cats_0.removeAt_za3lpa$(index_0);
      return true;
    }
    return false;
  };
  CatsRepository.prototype.updateCat_hu72qb$ = function (cat) {
    var $receiver = this.cats_0;
    var indexOfFirst$result;
    indexOfFirst$break: do {
      var tmp$;
      var index = 0;
      tmp$ = $receiver.iterator();
      while (tmp$.hasNext()) {
        var item = tmp$.next();
        if (Kotlin.equals(item.id, cat.id)) {
          indexOfFirst$result = index;
          break indexOfFirst$break;
        }
        index = index + 1 | 0;
      }
      indexOfFirst$result = -1;
    }
     while (false);
    var index_0 = indexOfFirst$result;
    if (index_0 >= 0) {
      var result = new Pair(this.cats_0.get_za3lpa$(index_0), cat);
      this.cats_0.set_wxm5ur$(index_0, cat);
      return result;
    }
    return null;
  };
  CatsRepository.prototype.createCat_hu72qb$ = function (cat) {
    cat.id = (new Date()).getTime().toString();
    this.cats_0.add_11rb$(cat);
    return cat;
  };
  CatsRepository.prototype.loadCats_0 = function () {
    var tmp$;
    tmp$ = FileLoader_getInstance().loadJsonDataFile_ytbaoo$('data/internal/cats/all_cats.json');
    if (tmp$ == null) {
      return Kotlin.kotlin.collections.ArrayList_init_ww73n8$();
    }
    var dto = tmp$;
    var cats = toMutableList(dto.cats);
    var tmp$_0;
    tmp$_0 = cats.iterator();
    while (tmp$_0.hasNext()) {
      var element = tmp$_0.next();
      Cat$Companion_getInstance().revive_11rb$(element);
    }
    return cats;
  };
  CatsRepository.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'CatsRepository', interfaces: []};
  function MockiferApi() {
    MockiferApi_instance = this;
  }
  function MockiferApi$JsonFileMockResponseController(serverSession) {
    RequestController.call(this, serverSession);
  }
  MockiferApi$JsonFileMockResponseController.prototype.respondToRequest_38q16g$ = function (route, request) {
    var tmp$;
    var headers = this.createResponseHeaders_xj066j$(request);
    tmp$ = FileLoader_getInstance().loadRawDataFile_61zpoe$('data/mocks/' + route.responseJsonFile);
    if (tmp$ == null) {
      return new Response(StatusCode_getInstance().INTERNAL_SERVER_ERROR, headers, toJson(new ErrorResponseBody("Route with id '" + route.routeId + "' was run, but the response json file " + route.responseJsonFile + ' failed to load.', this.serverSession)));
    }
    var responseBody = tmp$;
    return new Response(route.responseStatusCode, headers, responseBody);
  };
  MockiferApi$JsonFileMockResponseController.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'JsonFileMockResponseController', interfaces: [RequestController]};
  function MockiferApi$HtmlExampleController(serverSession) {
    RequestController.call(this, serverSession);
  }
  MockiferApi$HtmlExampleController.prototype.respondToRequest_38q16g$ = function (route, request) {
    var tmp$;
    var content = (tmp$ = FileLoader_getInstance().loadRawDataFile_61zpoe$('data/internal/html/example.html')) != null ? tmp$ : '';
    return new Response(StatusCode_getInstance().SUCCESS, this.createResponseHeaders_xj066j$(request), content);
  };
  MockiferApi$HtmlExampleController.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'HtmlExampleController', interfaces: [RequestController]};
  function MockiferApi$GetAllRoutesController(serverSession) {
    RequestController.call(this, serverSession);
  }
  MockiferApi$GetAllRoutesController.prototype.respondToRequest_38q16g$ = function (route, request) {
    var tmp$;
    var headers = this.createResponseHeaders_xj066j$(request);
    tmp$ = FileLoader_getInstance().loadRawDataFile_61zpoe$('routes.json');
    if (tmp$ == null) {
      return new Response(StatusCode_getInstance().INTERNAL_SERVER_ERROR, headers, toJson(new ErrorResponseBody("GetAllRoutesController failed to load 'routes.json'", this.serverSession)));
    }
    var responseBody = tmp$;
    return new Response(route.responseStatusCode, headers, responseBody);
  };
  MockiferApi$GetAllRoutesController.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'GetAllRoutesController', interfaces: [RequestController]};
  function MockiferApi$GetAllActiveMocks(serverSession) {
    RequestController.call(this, serverSession);
  }
  MockiferApi$GetAllActiveMocks.prototype.respondToRequest_38q16g$ = function (route, request) {
    var tmp$;
    var headers = this.createResponseHeaders_xj066j$(request);
    tmp$ = mockifer_getActiveMocks();
    if (tmp$ == null) {
      return new Response(StatusCode_getInstance().INTERNAL_SERVER_ERROR, headers, toJson(new ErrorResponseBody('GetAllActiveMocks failed to load fetch active mocks from host.', this.serverSession)));
    }
    var responseBody = tmp$;
    return new Response(route.responseStatusCode, headers, responseBody);
  };
  MockiferApi$GetAllActiveMocks.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'GetAllActiveMocks', interfaces: [RequestController]};
  var MockiferApi_instance = null;
  function mockiferProcessRequest(requestJson, routeJson) {
    return Server_getInstance().processRequest_jyasbz$(requestJson, routeJson);
  }
  function mockiferReset() {
    Server_getInstance().reset();
  }
  function Server() {
    Server_instance = this;
    this.serverSession_0 = this.serverSession_0;
    this.reset();
  }
  Server.prototype.reset = function () {
    this.serverSession_0 = new ServerSession();
  };
  Server.prototype.processRequest_jyasbz$ = function (requestJson, routeJson) {
    var tmp$, tmp$_0, tmp$_1;
    tmp$ = Request$Companion_getInstance().fromJson_pdl1vj$(requestJson);
    if (tmp$ == null) {
      return toJson(new Response(StatusCode_getInstance().BAD_REQUEST, this.serverSession_0.createResponseHeaders_q3ji22$(), toJson(new ErrorResponseBody('Malformed request received.', this.serverSession_0))));
    }
    var request = tmp$;
    tmp$_0 = Route$Companion_getInstance().fromJson_pdl1vj$(routeJson);
    if (tmp$_0 == null) {
      return toJson(new Response(StatusCode_getInstance().NOT_FOUND_ERROR, this.serverSession_0.createResponseHeaders_q3ji22$(), toJson(new ErrorResponseBody('No route found for request.', this.serverSession_0))));
    }
    var route = tmp$_0;
    if (!isMissingOrEmpty(route.responseJsonFile)) {
      return toJson((new MockiferApi$JsonFileMockResponseController(this.serverSession_0)).respondToRequest_38q16g$(route, request));
    }
    if (!isMissingOrEmpty(route.responseControllerId)) {
      if ((tmp$_1 = this.serverSession_0.createRequestController_61zpoe$(route.responseControllerId)) != null) {
        return toJson(tmp$_1.respondToRequest_38q16g$(route, request));
      }
    }
    return toJson(new Response(StatusCode_getInstance().INTERNAL_SERVER_ERROR, this.serverSession_0.createResponseHeaders_q3ji22$(request), toJson(new ErrorResponseBody('Route ' + route.routeId + ' was found but could not be successfully executed.', this.serverSession_0))));
  };
  Server.$metadata$ = {kind: Kotlin.Kind.OBJECT, simpleName: 'Server', interfaces: []};
  var Server_instance = null;
  function Server_getInstance() {
    if (Server_instance === null) {
      new Server();
    }
    return Server_instance;
  }
  function ServerSession(catsRepository) {
    if (catsRepository === void 0)
      catsRepository = new CatsRepository();
    this.catsRepository = catsRepository;
  }
  Object.defineProperty(ServerSession.prototype, 'formattedServerTime', {get: function () {
    return toLongDateString(new Date());
  }});
  ServerSession.prototype.createResponseHeaders_q3ji22$ = function (request) {
    if (request === void 0)
      request = null;
    var responseHeaders = Kotlin.kotlin.collections.LinkedHashMap_init_q3lmfv$();
    var key = Header$Companion_getInstance().HEADER_CONNECTION;
    var value = Header$Companion_getInstance().VALUE_CLOSE;
    responseHeaders.put_xwzc9p$(key, value);
    var key_0 = 'Mockifer';
    var value_0 = 'Powered by Mockifer!';
    responseHeaders.put_xwzc9p$(key_0, value_0);
    var $receiver = responseHeaders.entries;
    var destination = Kotlin.kotlin.collections.ArrayList_init_ww73n8$(Kotlin.kotlin.collections.collectionSizeOrDefault_ba2ldo$($receiver, 10));
    var tmp$;
    tmp$ = $receiver.iterator();
    while (tmp$.hasNext()) {
      var item = tmp$.next();
      destination.add_11rb$(new Header(item.key, item.value));
    }
    return Kotlin.kotlin.collections.copyToArray(destination);
  };
  ServerSession.prototype.createRequestController_61zpoe$ = function (controllerId) {
    var tmp$;
    tmp$ = controllerId.toLowerCase();
    if (Kotlin.equals(tmp$, 'mockifer.getallroutes'))
      return new MockiferApi$GetAllRoutesController(this);
    else if (Kotlin.equals(tmp$, 'mockifer.htmlexample'))
      return new MockiferApi$HtmlExampleController(this);
    else if (Kotlin.equals(tmp$, 'mockifer.getactivemocks'))
      return new MockiferApi$GetAllActiveMocks(this);
    else if (Kotlin.equals(tmp$, 'cats.getallcats'))
      return new CatsApi$GetAllCatsController(this);
    else if (Kotlin.equals(tmp$, 'cats.catdetails'))
      return new CatsApi$CatDetailsController(this);
    else if (Kotlin.equals(tmp$, 'cats.createcat'))
      return new CatsApi$CreateCatController(this);
    else if (Kotlin.equals(tmp$, 'cats.updatecat'))
      return new CatsApi$UpdateCatController(this);
    else if (Kotlin.equals(tmp$, 'cats.deletecat'))
      return new CatsApi$DeleteCatController(this);
    else
      return null;
  };
  ServerSession.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'ServerSession', interfaces: []};
  function DtoCompanion() {
  }
  DtoCompanion.prototype.revive_11rb$ = function (dto) {
  };
  DtoCompanion.prototype.fromJson_pdl1vj$ = function (json) {
    if (json == null) {
      return null;
    }
    try {
      var dto = JSON.parse(json);
      this.revive_11rb$(dto);
      if (!this.isValid_11rb$(dto)) {
        return null;
      }
      return dto;
    }
     catch (e) {
      mockifer_log("DtoCompanion.fromJson: Caught 'dynamic' exception: unable to parse json object: " + Kotlin.toString(json));
      return null;
    }
  };
  DtoCompanion.prototype.isValid_11rb$ = function (dto) {
    return true;
  };
  DtoCompanion.$metadata$ = {kind: Kotlin.Kind.INTERFACE, simpleName: 'DtoCompanion', interfaces: []};
  function toJson$lambda(f, v) {
    return v;
  }
  function toJson($receiver) {
    return JSON.stringify($receiver, toJson$lambda, 4);
  }
  function isMissing($receiver) {
    return $receiver == null || Kotlin.equals($receiver, undefined);
  }
  function isMissingOrEmpty($receiver) {
    return isMissing($receiver) || ($receiver != null ? $receiver.length : null) === 0;
  }
  function orDefault($receiver, default_0) {
    return isMissing($receiver) ? default_0 : $receiver != null ? $receiver : Kotlin.throwNPE();
  }
  function getFirstMatch($receiver, input) {
    var tmp$, tmp$_0, tmp$_1;
    return (tmp$_1 = (tmp$_0 = (tmp$ = $receiver.find_905azu$(input)) != null ? tmp$.destructured : null) != null ? tmp$_0.match.groupValues.get_za3lpa$(1) : null) != null ? tmp$_1 : '';
  }
  function toShortDateString($receiver) {
    return mockifer_getFormattedDateShort($receiver);
  }
  function toLongDateString($receiver) {
    return mockifer_getFormattedDateLong($receiver);
  }
  function addDays($receiver, days) {
    return mockifer_addDaysToDate($receiver, days);
  }
  function FileLoader() {
    FileLoader_instance = this;
  }
  FileLoader.prototype.loadJsonDataFile_ytbaoo$ = function (filePath) {
    var tmp$, tmp$_0;
    try {
      tmp$_0 = (tmp$ = mockifer_loadDataFile(filePath)) != null ? tmp$ : '';
      var json = MagicTokenResolver_getInstance().resolveMagicTokens_61zpoe$(tmp$_0);
      return JSON.parse(json);
    }
     catch (e) {
      return null;
    }
  };
  FileLoader.prototype.loadRawDataFile_61zpoe$ = function (filePath) {
    var tmp$, tmp$_0;
    try {
      tmp$_0 = (tmp$ = mockifer_loadDataFile(filePath)) != null ? tmp$ : '';
      return MagicTokenResolver_getInstance().resolveMagicTokens_61zpoe$(tmp$_0);
    }
     catch (e) {
      return null;
    }
  };
  FileLoader.$metadata$ = {kind: Kotlin.Kind.OBJECT, simpleName: 'FileLoader', interfaces: []};
  var FileLoader_instance = null;
  function FileLoader_getInstance() {
    if (FileLoader_instance === null) {
      new FileLoader();
    }
    return FileLoader_instance;
  }
  function MagicTokenResolver() {
    MagicTokenResolver_instance = this;
    var $receiver = '\\{\\{([^}]*?)}}';
    this.magicTokenPattern_0 = Kotlin.kotlin.text.Regex_61zpoe$($receiver);
  }
  MagicTokenResolver.prototype.resolveMagicTokens_61zpoe$ = function (input) {
    var regex = this.magicTokenPattern_0;
    var replace_20wsma$result;
    replace_20wsma$break: do {
      var match = regex.find_905azu$(input);
      if (match == null) {
        replace_20wsma$result = input.toString();
        break replace_20wsma$break;
      }
      var lastStart = 0;
      var length = input.length;
      var sb = Kotlin.kotlin.text.StringBuilder_init_za3lpa$(length);
      do {
        var foundMatch = match != null ? match : Kotlin.throwNPE();
        sb.append_ezbsdh$(input, lastStart, foundMatch.range.start);
        sb.append_gw00v9$(this.resolveToken_0(foundMatch.value));
        lastStart = foundMatch.range.endInclusive + 1 | 0;
        match = foundMatch.next();
      }
       while (lastStart < length && match != null);
      if (lastStart < length) {
        sb.append_ezbsdh$(input, lastStart, length);
      }
      replace_20wsma$result = sb.toString();
    }
     while (false);
    return replace_20wsma$result;
  };
  MagicTokenResolver.prototype.resolveToken_0 = function (magicToken) {
    var endIndex = magicToken.length - 2 | 0;
    var content = magicToken.substring(2, endIndex);
    if (startsWith(content, 'today')) {
      return this.resolveToday_0(content);
    }
    return magicToken;
  };
  MagicTokenResolver.prototype.resolveToday_0 = function (content) {
    var date = new Date();
    date = this.adjustDateForOperator_0(date, content, '-');
    date = this.adjustDateForOperator_0(date, content, '+');
    if (startsWith(content, 'todayTime')) {
      return toLongDateString(date);
    }
    return toShortDateString(date);
  };
  MagicTokenResolver.prototype.adjustDateForOperator_0 = function (date, content, operator) {
    var tmp$;
    var operatorIndex = lastIndexOf(content, operator);
    if (operatorIndex > 0) {
      var endIndex = content.length;
      if ((tmp$ = toIntOrNull(content.substring(operatorIndex, endIndex))) != null) {
        return addDays(date, tmp$);
      }
    }
    return date;
  };
  MagicTokenResolver.$metadata$ = {kind: Kotlin.Kind.OBJECT, simpleName: 'MagicTokenResolver', interfaces: []};
  var MagicTokenResolver_instance = null;
  function MagicTokenResolver_getInstance() {
    if (MagicTokenResolver_instance === null) {
      new MagicTokenResolver();
    }
    return MagicTokenResolver_instance;
  }
  function ErrorResponseBody(errorDescription, serverSession) {
    ResponseBody.call(this, serverSession);
    this.errorDescription = errorDescription;
  }
  ErrorResponseBody.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'ErrorResponseBody', interfaces: [ResponseBody]};
  function Header(key, value) {
    Header$Companion_getInstance();
    this.key = key;
    this.value = value;
  }
  function Header$Companion() {
    Header$Companion_instance = this;
    this.HEADER_CONNECTION = 'Connection';
    this.VALUE_CLOSE = 'close';
  }
  Header$Companion.$metadata$ = {kind: Kotlin.Kind.OBJECT, simpleName: 'Companion', interfaces: []};
  var Header$Companion_instance = null;
  function Header$Companion_getInstance() {
    if (Header$Companion_instance === null) {
      new Header$Companion();
    }
    return Header$Companion_instance;
  }
  Header.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'Header', interfaces: []};
  function Request(method, uri, headers, queryString, body) {
    Request$Companion_getInstance();
    this.method = method;
    this.uri = uri;
    this.headers = headers;
    this.queryString = queryString;
    this.body = body;
    this.queryParams = Kotlin.kotlin.collections.emptyMap_q3lmfv$();
  }
  function Request$Companion() {
    Request$Companion_instance = this;
  }
  Request$Companion.prototype.revive_11rb$ = function (dto) {
    dto.method = orDefault(dto.method, '');
    dto.uri = orDefault(dto.uri, '');
    dto.headers = orDefault(dto.headers, []);
    dto.queryString = orDefault(dto.queryString, '');
    dto.body = orDefault(dto.body, '');
    var map = Kotlin.kotlin.collections.LinkedHashMap_init_q3lmfv$();
    var tmp$;
    tmp$ = split(dto.queryString, ['&']).iterator();
    while (tmp$.hasNext()) {
      var element = tmp$.next();
      var pair = split(element, ['=']);
      if (pair.size === 2) {
        var key = pair.get_za3lpa$(0);
        var value = pair.get_za3lpa$(1);
        map.put_xwzc9p$(key, value);
      }
    }
    dto.queryParams = toMap(map);
  };
  Request$Companion.prototype.isValid_11rb$ = function (dto) {
    var tmp$ = dto.method.length > 0;
    if (tmp$) {
      tmp$ = dto.uri.length > 0;
    }
    return tmp$;
  };
  Request$Companion.$metadata$ = {kind: Kotlin.Kind.OBJECT, simpleName: 'Companion', interfaces: [DtoCompanion]};
  var Request$Companion_instance = null;
  function Request$Companion_getInstance() {
    if (Request$Companion_instance === null) {
      new Request$Companion();
    }
    return Request$Companion_instance;
  }
  function RequestController(serverSession) {
    this.serverSession = serverSession;
  }
  RequestController.prototype.createResponseHeaders_xj066j$ = function (request) {
    return this.serverSession.createResponseHeaders_q3ji22$(request);
  };
  RequestController.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'RequestController', interfaces: []};
  var RequestMethod_instance = null;
  function Response(statusCode, headers, body) {
    Response$Companion_getInstance();
    this.statusCode = statusCode;
    this.headers = headers;
    this.body = body;
  }
  function Response$Companion() {
    Response$Companion_instance = this;
  }
  Response$Companion.prototype.revive_11rb$ = function (dto) {
    dto.statusCode = orDefault(dto.statusCode, StatusCode_getInstance().BAD_REQUEST);
    dto.headers = orDefault(dto.headers, []);
    dto.body = orDefault(dto.body, '{}');
  };
  Response$Companion.$metadata$ = {kind: Kotlin.Kind.OBJECT, simpleName: 'Companion', interfaces: [DtoCompanion]};
  var Response$Companion_instance = null;
  function Response$Companion_getInstance() {
    if (Response$Companion_instance === null) {
      new Response$Companion();
    }
    return Response$Companion_instance;
  }
  Response.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'Response', interfaces: []};
  function ResponseBody(serverSession) {
    this.serverTime = serverSession.formattedServerTime;
  }
  ResponseBody.$metadata$ = {kind: Kotlin.Kind.CLASS, simpleName: 'ResponseBody', interfaces: []};
  function Route(routeId, isInternal, routeDisplayName, routeDescription, requestMethod, requestUri, requestOverrideRouteId, requestQueryStringContains, requestQueryStringEquals, requestBodyJsonPath, requestBodyJsonPathContains, requestBodyJsonPathEquals, responseStatusCode, responseJsonFile, responseControllerId) {
    Route$Companion_getInstance();
    this.routeId = routeId;
    this.isInternal = isInternal;
    this.routeDisplayName = routeDisplayName;
    this.routeDescription = routeDescription;
    this.requestMethod = requestMethod;
    this.requestUri = requestUri;
    this.requestOverrideRouteId = requestOverrideRouteId;
    this.requestQueryStringContains = requestQueryStringContains;
    this.requestQueryStringEquals = requestQueryStringEquals;
    this.requestBodyJsonPath = requestBodyJsonPath;
    this.requestBodyJsonPathContains = requestBodyJsonPathContains;
    this.requestBodyJsonPathEquals = requestBodyJsonPathEquals;
    this.responseStatusCode = responseStatusCode;
    this.responseJsonFile = responseJsonFile;
    this.responseControllerId = responseControllerId;
  }
  function Route$Companion() {
    Route$Companion_instance = this;
  }
  Route$Companion.prototype.revive_11rb$ = function (dto) {
    dto.routeId = orDefault(dto.routeId, '');
    dto.isInternal = orDefault(dto.isInternal, false);
    dto.routeDisplayName = orDefault(dto.routeDisplayName, '');
    dto.routeDescription = orDefault(dto.routeDescription, '');
    dto.requestMethod = orDefault(dto.requestMethod, '');
    dto.requestUri = orDefault(dto.requestUri, '');
    dto.requestOverrideRouteId = orDefault(dto.requestOverrideRouteId, '');
    dto.requestQueryStringContains = orDefault(dto.requestQueryStringContains, '');
    dto.requestQueryStringEquals = orDefault(dto.requestQueryStringEquals, '');
    dto.requestBodyJsonPath = orDefault(dto.requestBodyJsonPath, '');
    dto.requestBodyJsonPathContains = orDefault(dto.requestBodyJsonPathContains, '');
    dto.requestBodyJsonPathEquals = orDefault(dto.requestBodyJsonPathEquals, '');
    dto.responseStatusCode = orDefault(dto.responseStatusCode, 200);
    dto.responseJsonFile = orDefault(dto.responseJsonFile, '');
    dto.responseControllerId = orDefault(dto.responseControllerId, '');
  };
  Route$Companion.$metadata$ = {kind: Kotlin.Kind.OBJECT, simpleName: 'Companion', interfaces: [DtoCompanion]};
  var Route$Companion_instance = null;
  function Route$Companion_getInstance() {
    if (Route$Companion_instance === null) {
      new Route$Companion();
    }
    return Route$Companion_instance;
  }
  function StatusCode() {
    StatusCode_instance = this;
    this.SUCCESS = 200;
    this.INTERNAL_SERVER_ERROR = 500;
    this.BAD_REQUEST = 400;
    this.NOT_FOUND_ERROR = 404;
    this.UNAUTHORIZED_ERROR = 403;
  }
  StatusCode.$metadata$ = {kind: Kotlin.Kind.OBJECT, simpleName: 'StatusCode', interfaces: []};
  var StatusCode_instance = null;
  function StatusCode_getInstance() {
    if (StatusCode_instance === null) {
      new StatusCode();
    }
    return StatusCode_instance;
  }
  Object.defineProperty(Cat, 'Companion', {get: Cat$Companion_getInstance});
  var package$mockifer = _.mockifer || (_.mockifer = {});
  var package$api = package$mockifer.api || (package$mockifer.api = {});
  var package$cats = package$api.cats || (package$api.cats = {});
  package$cats.Cat = Cat;
  CatsApi.prototype.GetAllCatsController = CatsApi$GetAllCatsController;
  CatsApi.prototype.CatDetailsController = CatsApi$CatDetailsController;
  CatsApi.prototype.CreateCatController = CatsApi$CreateCatController;
  CatsApi.prototype.DeleteCatController = CatsApi$DeleteCatController;
  CatsApi.prototype.UpdateCatController = CatsApi$UpdateCatController;
  package$cats.CatsRepository = CatsRepository;
  MockiferApi.prototype.JsonFileMockResponseController = MockiferApi$JsonFileMockResponseController;
  MockiferApi.prototype.HtmlExampleController = MockiferApi$HtmlExampleController;
  MockiferApi.prototype.GetAllRoutesController = MockiferApi$GetAllRoutesController;
  MockiferApi.prototype.GetAllActiveMocks = MockiferApi$GetAllActiveMocks;
  _.mockiferProcessRequest = mockiferProcessRequest;
  _.mockiferReset = mockiferReset;
  var package$core = package$mockifer.core || (package$mockifer.core = {});
  Object.defineProperty(package$core, 'Server', {get: Server_getInstance});
  package$core.ServerSession = ServerSession;
  var package$framework = package$core.framework || (package$core.framework = {});
  package$framework.DtoCompanion = DtoCompanion;
  package$framework.toJson_s8jyvk$ = toJson;
  package$framework.isMissing_mzud1t$ = isMissing;
  package$framework.isMissingOrEmpty_5cw0du$ = isMissingOrEmpty;
  package$framework.orDefault_w3ol5z$ = orDefault;
  package$framework.getFirstMatch_dz5iji$ = getFirstMatch;
  package$framework.toShortDateString_t5kl13$ = toShortDateString;
  package$framework.toLongDateString_t5kl13$ = toLongDateString;
  package$framework.addDays_79if1n$ = addDays;
  Object.defineProperty(package$framework, 'FileLoader', {get: FileLoader_getInstance});
  Object.defineProperty(package$framework, 'MagicTokenResolver', {get: MagicTokenResolver_getInstance});
  var package$model = package$core.model || (package$core.model = {});
  package$model.ErrorResponseBody = ErrorResponseBody;
  Object.defineProperty(Header, 'Companion', {get: Header$Companion_getInstance});
  package$model.Header = Header;
  Object.defineProperty(Request, 'Companion', {get: Request$Companion_getInstance});
  package$model.Request = Request;
  package$model.RequestController = RequestController;
  Object.defineProperty(Response, 'Companion', {get: Response$Companion_getInstance});
  package$model.Response = Response;
  package$model.ResponseBody = ResponseBody;
  Object.defineProperty(Route, 'Companion', {get: Route$Companion_getInstance});
  package$model.Route = Route;
  Object.defineProperty(package$model, 'StatusCode', {get: StatusCode_getInstance});
  Cat$Companion.prototype.fromJson_pdl1vj$ = DtoCompanion.prototype.fromJson_pdl1vj$;
  Request$Companion.prototype.fromJson_pdl1vj$ = DtoCompanion.prototype.fromJson_pdl1vj$;
  Response$Companion.prototype.fromJson_pdl1vj$ = DtoCompanion.prototype.fromJson_pdl1vj$;
  Response$Companion.prototype.isValid_11rb$ = DtoCompanion.prototype.isValid_11rb$;
  Route$Companion.prototype.isValid_11rb$ = DtoCompanion.prototype.isValid_11rb$;
  Route$Companion.prototype.fromJson_pdl1vj$ = DtoCompanion.prototype.fromJson_pdl1vj$;
  return _;
}(typeof content_main === 'undefined' ? {} : content_main, kotlin);
