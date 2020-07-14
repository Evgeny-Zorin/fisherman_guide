new QWebChannel(qt.webChannelTransport,
 (channel) => {
 var server = channel.objects.server;
 window.server = server;
 //alert("Channel set");
});
