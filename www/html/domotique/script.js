function actionLampe(lampe) {
 if (lampe == 0){
 	var idlampe = "lampe_bureau";
 }
  if (lampe == 1){
 	var idlampe = "lampe_lit";
 }
  if (lampe == 2){
 	var idlampe = "lampe_3";
 }
 var xhr = new XMLHttpRequest();
 var etat = document.getElementById(idlampe).value;
 if(etat == "1"){
 	var commande = "0";
 }
 if(etat == "0"){
 	var commande = "1";
 }
 var adresse = document.URL;
 console.log(lampe + commande)
 xhr.open('GET', adresse + 'commandes.php?' + "lampe=" + lampe +"&action=" + commande + "&key=your_key");
 xhr.send(null);
if(commande == "1"){
	document.getElementById(idlampe).value =  "1";
	}
if(commande == "0"){
	document.getElementById(idlampe).value = "0";
	}
}
function actionGroupe(action) {
	 var xhr = new XMLHttpRequest();
	 var adresse = document.URL;
	  var lampe = "G";
	  if (action == 1 || action == 0) {
	  	var commande = action;
	  }
	   console.log(lampe  + commande)
	  xhr.open('GET', adresse + 'commandes.php?' + "lampe=" + lampe +"&action=" + commande + "&key=your_key");
	  xhr.send(null);
	  if (commande == 1) {
	  	newcheck = true;
	  	newvalue = "1"
	  }
	  if (commande == 0) {
	  	newcheck = false;
	  	newvalue = "0"
	  }
	  document.getElementById("lampe_bureau").checked = newcheck;
	  document.getElementById("lampe_lit").checked = newcheck;
	  document.getElementById("lampe_3").checked = newcheck;
	   document.getElementById("lampe_bureau").value = newvalue;
	  document.getElementById("lampe_lit").value = newvalue;
	  document.getElementById("lampe_3").value = newvalue;
}

function actionVolet(volet, action) {
 var xhr = new XMLHttpRequest();
var adresse = document.URL;
xhr.open('GET', adresse + 'commandes.php?' + "volet=" + volet +"&action=" + action + "&key=your_key");
xhr.send(null);
console.log(volet + action)
}
