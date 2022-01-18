const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector("#content-sign-in");
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector("#authentication-bar");

// Elements for sensor readings
const tempElement = document.getElementById("temp");
const humElement = document.getElementById("hum");
const nodeElement = document.getElementById("node");
//const udaraElement = document.getElementById("udara");
const atitudeElement = document.getElementById("atitude");
const stresElement = document.getElementById("stres");

// MANAGE LOGIN/LOGOUT UI
const setupUI = (user) => {
  if (user) {
    //toggle UI elements
    loginElement.style.display = 'none';
    contentElement.style.display 
    = 'block';
    authBarElement.style.display ='block';
    userDetailsElement.style.display ='block';
    userDetailsElement.innerHTML = user.email;

    // get user UID to get data from database
    var uid = user.uid;
    console.log(uid);
//uid.toString()
    // Database paths (with user UID)
    var dbPathTemp = 'HealthCow/' + '3600/'+ '/suhu';
    var dbPathHum = 'HealthCow/' + '3600/' + '/kelembaban';
    var dbPathNode = 'HealthCow/' + '3600/' + '/node';
   // var dbPathUdara = 'HealthCow/' + '3600/' + '/udara';
    var dbPathAtitude = 'HealthCow/' + '3600/' + '/keadaan';
    var dbPathStres = 'HealthCow/' + '3600/' + '/stres';
    // Database references
    var dbRefTemp = firebase.database().ref().child(dbPathTemp);
    var dbRefHum = firebase.database().ref().child(dbPathHum);
    var dbRefNode = firebase.database().ref().child(dbPathNode);
  //  var dbRefUdara = firebase.database().ref().child(dbPathUdara);
    var dbRefAtitude = firebase.database().ref().child(dbPathAtitude);
    var dbRefStres = firebase.database().ref().child(dbPathStres);

    // Update page with new readings
    dbRefTemp.on('value', snap => {
      tempElement.innerText = snap.val();
    });

    dbRefHum.on('value', snap => {
      humElement.innerText = snap.val();
    });

    dbRefNode.on('value', snap => {
      nodeElement.innerText = snap.val();
    });
    // dbRefUdara.on('value', snap => {
    //   udaraElement.innerText = snap.val();
    // });
    dbRefAtitude.on('value', snap => {
      atitudeElement.innerText = snap.val();
    });
    dbRefStres.on('value', snap => {
      stresElement.innerText = snap.val();
    });

  // if user is logged out
  } else{
    // toggle UI elements
    loginElement.style.display = 'block';
    authBarElement.style.display ='none';
    userDetailsElement.style.display ='none';
    contentElement.style.display = 'none';
  }
}