new QWebChannel(qt.webChannelTransport,
 function(channel){
 var qProxy = channel.objects.qProxy;
 window.foo = qProxy;
 });
