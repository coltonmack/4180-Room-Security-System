<!DOCTYPE html>
<html>
<head>
<link rel="stylesheet" type="text/css" href="theme.css">
<title>4180 Webcam Monitoring</title>
<script>

var img_name="";
var ultrasonicReading = "0";
var microphoneReading = "";
var count = 0;

function checkWebcam(){
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange=function(){
	if (xmlhttp.readyState==4 && xmlhttp.status==200){
			img_name=xmlhttp.responseText;
		}
	}
	//Get the latest info from 'ajax_info.txt'
	xmlhttp.open("GET","ajax_info.txt?_=" + new Date().getTime(), true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();
	//Refresh the image only if it has a new name
	if(document.getElementById("hiddenData1").innerHTML != img_name){
		document.getElementById("webcam_img").src = img_name;
		document.getElementById("hiddenData1").innerHTML = img_name;
	}
}
		
function updateWebcam(){
	var xmlhttp=new XMLHttpRequest();
	xmlhttp.onreadystatechange=function(){
	if (xmlhttp.readyState==4 && xmlhttp.status==200){
		}
	}
	//Take a still image
	xmlhttp.open("GET","updateWebcam.php?_=" + new Date().getTime(), true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();
}

function soundAlarm(){
	var xmlhttp=new XMLHttpRequest();
	xmlhttp.onreadystatechange=function(){
	if (xmlhttp.readyState==4 && xmlhttp.status==200){
		}
	}
	//Take a still image
	xmlhttp.open("GET","soundAlarm.php?_=" + new Date().getTime(), true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();
}

function checkSensor(){
	var xmlhttp = new XMLHttpRequest();
	return new Promise(function(resolve, reject) {	
	xmlhttp.onreadystatechange=function(){
	if (xmlhttp.readyState==4 && xmlhttp.status==200){
			ultrasonicReading = xmlhttp.responseText;
			if (ultrasonicReading != "") {
				if (ultrasonicReading == 0) {
					document.getElementById("DistanceDiv").innerHTML = "No Object Detected";
					document.getElementById("DistanceDiv").style.background = "#174cd1";  
					resolve("No objects detected");		
				} else if (ultrasonicReading == 1) {
					document.getElementById("DistanceDiv").innerHTML = "Object Detected!";
					document.getElementById("DistanceDiv").style.background = "#ed0707";	
					resolve("Object detected");
				}
				resolve("Ultrasonic not read");
			}
		}
	}
	//Get the latest info from 'ajax_info.txt'
	xmlhttp.open("GET","checkUltrasonic.php?_=" + new Date().getTime(), true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();
	});
}

function checkMicrophone(){
	var xmlhttp = new XMLHttpRequest();
	return new Promise(function(resolve, reject) {	
	xmlhttp.onreadystatechange=function(){
	if (xmlhttp.readyState==4 && xmlhttp.status==200){
			microphoneReading = xmlhttp.responseText;
			if (microphoneReading != "") {
				if (microphoneReading == 0) {
					document.getElementById("MicDiv").innerHTML = "No Sound Detected";
					document.getElementById("MicDiv").style.background = "#174cd1";  
					resolve("No sound detected");		
				} else if (microphoneReading == 1) {
					document.getElementById("MicDiv").innerHTML = "Sound Detected!";
					document.getElementById("MicDiv").style.background = "#ed0707";	
					resolve("Sound detected");
				}
				resolve("Mic not read");
			}
		}
	}
	//Get the latest info from 'ajax_info.txt'
	xmlhttp.open("GET","checkMic.php?_=" + new Date().getTime(), true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();
	});
}

function refresh(timeoutPeriod){
	checkWebcam();
	count = 0;
	checkMicrophone()
	.then(function (result) {
		console.log(result);
		if (result == "Sound detected")
		count += 1;
		checkSensor()
			.then(function (result) {
			console.log(result);
			if (result == "Object detected")
			count += 1;
			if (count > 0) {
				updateWebcam();
				soundAlarm();			
			}
		});
	});
	setTimeout("refresh(2000);",timeoutPeriod);
}

window.addEventListener("load", myInit, true);

function myInit() {
	refresh(2000);
}

</script>
</head>
<body>
<br>
<div>
	<center>
		<h1>The world's most sophisticated intrusion detection system</h1>
	</center>
</div>
<br>
<br>
<div>
	<div class = "buttonWrapper">
		<button class="buttons" id="intruderButton" onclick='updateWebcam()'>Snap the Intruder!</button>
		<button class="buttons" onclick='soundAlarm()'>Ring the Alarm!</button>
	</div>
</div>
<br>
<br>
<div>
	<center>
		<h1>Our last culprit:</h1>
	</center>
	<center>
		<img id="webcam_img" src="./img/image1.jpg" alt="" height="200" width= "400">
	</center>
</div>
<div id="hiddenData1" style="display:none;"></div>
<br>
<br>
<br>
<div id="DistanceSpeakerWrapper">
<div class= "flex-child" id="DistanceDiv">No Object Detected</div>
<div class= "flex-child" id="MicDiv">No Sound Detected</div>
</div>
</body>
</html>