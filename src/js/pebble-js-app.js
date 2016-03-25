Pebble.addEventListener('showConfiguration', function (e) {
    // paxina web de configuracion da esfera
    Pebble.openURL('https://dl.dropboxusercontent.com/s/xsbxc2s8o68mahi/config.html');
});

Pebble.addEventListener('webviewclosed', function (e) {
    var json = JSON.parse(decodeURIComponent(e.response));

    var options = {
        "MOSTRAR_DIA": '' + json.dia,
        "MOSTRAR_DATA": '' + json.data,
        "MOSTRAR_CARGA": '' + json.carga,
        "INVERTIR_CORES": '' + json.invertir,
    };

  Pebble.sendAppMessage(options, function(e) {}, function(e) {
        console.log('Fallo na actualizacion dos axustes: ' + JSON.stringify(e));
    });
});