ws = new WebSocket("ws://" + window.location.hostname + "/ws");
ws.onmessage = function(e) {
    document.getElementById("incomingMsgOutput").value += e.data;
};