Pebble.addEventListener("ready", function() {
    sendMsg({ '11': 'true' });
    Pebble.addEventListener('appmessage',
      function(e) {
        if(Object.keys(e.payload)[0] === '22') {
            sendMsg({ '22': true, 'name': "test", 'id': 1234, "subtitle": "This is a test.", "type": "private"});
        }

        if(Object.keys(e.payload)[0] === '23') {
          sendMsg({ '23': 0});
        }
        console.log('Received message: ' + JSON.stringify(e.payload));
      });
});

function sendMsg(obj) {
    var transactionId = Pebble.sendAppMessage(obj,
      function(e) {
        console.log('Successfully delivered message with transactionId='
          + e.data.transactionId);
      },
      function(e) {
        console.log('Unable to deliver message with transactionId='
          + e.data.transactionId
          + ' Error is: ' + e.error.message);
      });
}