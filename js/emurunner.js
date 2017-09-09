var mainCanvas = null;
var showAsMinimal = false;
var keyZones = [
  ["right", [39]],
  ["left", [37]],
  ["up", [38]],
  ["down", [40]],
  ["a", [65]],
  ["b", [83]],
  ["select", [16]],
  ["start", [13]]
];

var cout = console.log.bind(this);

function startGame (blob) {
  var binaryHandle = new FileReader();
  binaryHandle.onload = function () {
    if (this.readyState === 2) {
      start(mainCanvas, this.result);
    }
  };
  binaryHandle.readAsBinaryString(blob);
};

function loadViaXHR () {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "./glitch.gb");
  xhr.responseType = "blob";
  xhr.onload = function () {
    startGame(new Blob([this.response], { type: "text/plain" }));
  };
  xhr.send();
};

function windowingInitialize() {
  mainCanvas = document.getElementById("mainCanvas");
  registerGUIEvents();
  loadViaXHR();
}

window.addEventListener("DOMContentLoaded", windowingInitialize);
function registerGUIEvents() {
  addEvent("keydown", document, keyDown);
  addEvent("keyup", document,  function (event) {
    keyUp(event);
  });
  addEvent("resize", window, initNewCanvasSize);
}
function keyDown(event) {
  var keyCode = event.keyCode;
  cout(keyCode);
  var keyMapLength = keyZones.length;
  for (var keyMapIndex = 0; keyMapIndex < keyMapLength; ++keyMapIndex) {
    var keyCheck = keyZones[keyMapIndex];
    var keysMapped = keyCheck[1];
    var keysTotal = keysMapped.length;
    for (var index = 0; index < keysTotal; ++index) {
      if (keysMapped[index] == keyCode) {
        GameBoyKeyDown(keyCheck[0]);
        event.preventDefault();
      }
    }
  }
}
function keyUp(event) {
  var keyCode = event.keyCode;
  var keyMapLength = keyZones.length;
  for (var keyMapIndex = 0; keyMapIndex < keyMapLength; ++keyMapIndex) {
    var keyCheck = keyZones[keyMapIndex];
    var keysMapped = keyCheck[1];
    var keysTotal = keysMapped.length;
    for (var index = 0; index < keysTotal; ++index) {
      if (keysMapped[index] == keyCode) {
        console.log(keyCheck[0])
        GameBoyKeyUp(keyCheck[0]);
        event.preventDefault();
      }
    }
  }
}

function addEvent(sEvent, oElement, fListener) {
  oElement.addEventListener(sEvent, fListener, false);
}
function removeEvent(sEvent, oElement, fListener) {
    oElement.removeEventListener(sEvent, fListener, false);
    oElement.detachEvent("on" + sEvent, fListener);	//Pity for IE.
}
