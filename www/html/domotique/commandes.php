<?php 
$bdd = new PDO('mysql:host=localhost;dbname=domotique;charset=utf8', 'phpmyadmin', 'your_password');
	if (isset($_GET["lampe"])){

	if (isset($_GET["action"])){
	if (isset($_GET["key"])) {
	

	$action = $_GET["action"];
	$lampe = $_GET["lampe"];
	$key = $_GET["key"];
	if($lampe == "0" OR $lampe == "1" OR $lampe == "2"){
		if($action == "0" OR $action == "1"){
			if($key=="your_key"){

$output = shell_exec('/var/www/commande.sh L' . $lampe . $action);
	$req = $bdd->query('UPDATE lampes SET etat_lampe="' . $action . '" WHERE id_lampe=' . $lampe);
echo "OK LAMPE " . $lampe . " " . $action ;
echo $output;
					}
	else{
		echo "CLE UNITE INCORRECTE";
	}
				}
			
		}
			elseif ($lampe =="G") {
				if ($action=="0" OR $action=="1") {
					if ($key=="your_key") {
					$output = shell_exec('/var/www/commande.sh L' . $lampe . $action);	
					$req = $bdd->query('UPDATE lampes SET etat_lampe="' . $action . '" WHERE id_lampe=0');
					$req = $bdd->query('UPDATE lampes SET etat_lampe="' . $action . '" WHERE id_lampe=1');
					$req = $bdd->query('UPDATE lampes SET etat_lampe="' . $action . '" WHERE id_lampe=2');
					echo "OK GROUPE " . $action;
					echo $output;
					}
					else{
						echo "CLE GROUPE INCORRECTE";
					}

				}
			}
		else{
			echo "Lampe incorrecte";
		}

	}
}
}
elseif(isset($_GET["volet"])){
	if (isset($_GET["action"])){
	if (isset($_GET["key"])) {

	$action = $_GET["action"];
	$volet = $_GET["volet"];
	$key = $_GET["key"];

		if ($volet=="R"){
		if($action == "M" OR $action == "S" OR $action == "B"){
		if($key=="your_key"){

			$output = shell_exec('/var/www/commande.sh V' . $volet . $action);
			echo $output;
			$req = $bdd->query('UPDATE volets SET etat_volet="' . $action . '" WHERE id_volet="' . $volet . '"');
			echo "SUCCESS";

} else {echo "CLE VOLET INVALIDE";}
}
} 
else{echo 'VOLET INCORRECT';}
		}
	}
}
?>
