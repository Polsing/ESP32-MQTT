client = new Paho.MQTT.Client("broker.emqx.io", Number(8083), "mqttx_05779cf3");
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

client.connect({onSuccess:onConnect});

function onConnect() {
  client.subscribe("formData");
}

function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

function onMessageArrived(message) {
    const payloadString = message.payloadString;
    try {
      // แปลงสตริง JSON เป็นอ็อบเจ็กต์
      const data = JSON.parse(payloadString);
      // เข้าถึงข้อมูลที่คุณต้องการ
      const celsius = data.temperature_C.toFixed(2);
      const fahrenheit = data.temperature_F.toFixed(2);
      const humidity = data.humidity.toFixed(2);
      // ทำบางอย่างกับข้อมูลที่ได้รับที่นี่
      document.getElementById('Celsius').innerText = "Celsius: " + celsius + " °C";
      document.getElementById('Fahrenheit').innerText = "Fahrenheit: "+ fahrenheit + " °F";
      document.getElementById('Humidity').innerText = "umidity: "+ humidity + " %";
     
    } catch (error) {
      console.error("Error parsing JSON:", error);
    }
}

function Onled1(){
    const led1 = document.getElementById('led1').checked;
    const jsonled1 = { LED1: led1 };
    
    if(led1){
    const jsonString = JSON.stringify(jsonled1);
    const ledMessage = new Paho.MQTT.Message(jsonString);
    ledMessage.destinationName = "onLED";
    client.send(ledMessage);
    }else{
      const jsonString = JSON.stringify(jsonled1);
      const ledMessage = new Paho.MQTT.Message(jsonString);
      ledMessage.destinationName = "onLED";
     client.send(ledMessage);
    }
}
function Onled2(){
    const led2 = document.getElementById('led2').checked;
    const jsonled2 = { LED2: led2 };
    
    if(led2){
    const jsonString = JSON.stringify(jsonled2);
    const ledMessage = new Paho.MQTT.Message(jsonString);
    ledMessage.destinationName = "onLED";
    client.send(ledMessage);
    }else{
      const jsonString = JSON.stringify(jsonled2);
      const ledMessage = new Paho.MQTT.Message(jsonString);
      ledMessage.destinationName = "onLED";
     client.send(ledMessage);
    }
}


