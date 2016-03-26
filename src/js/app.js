Pebble.addEventListener('showConfiguration', function (e) {
    Pebble.openURL('https://rawgit.com/pazos/reloxo-pebble/master/config/index.html');
});

Pebble.addEventListener('webviewclosed', function (e) {
    var json = JSON.parse(decodeURIComponent(e.response));
    var options = {
        "MOSTRAR_DIA": '' + json.dia,
        "MOSTRAR_DATA": '' + json.data,
        "MOSTRAR_CARGA": '' + json.carga,
        "INVERTIR_CORES": '' + json.invertir, 
    };
  Pebble.sendAppMessage(options, function(e){}, function(e){console.log('Settings update failed: ' + JSON.stringify(e));});
});
