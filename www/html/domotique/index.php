<?php 
$bdd = new PDO('mysql:host=localhost;dbname=domotique;charset=utf8', 'phpmyadmin', 'your_password');
$etat_lampe0 = $bdd->query('SELECT etat_lampe FROM lampes WHERE id_lampe=0')->fetch();
$etat_lampe1 = $bdd->query('SELECT etat_lampe FROM lampes WHERE id_lampe=1')->fetch();
$etat_lampe2 = $bdd->query('SELECT etat_lampe FROM lampes WHERE id_lampe=2')->fetch();
 ?>
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>Domotique</title>
	<link rel="stylesheet" type="text/css" href="style.css">
	<link rel="stylesheet" type="text/css" href="style_template.css">
	<script type="text/javascript" src="script.js"></script>
</head>
<body>
    <div id="page_content">
<div class="lampes">
        <div class="lampe_block">
<h1>Lampe Bureau</h1>
<div class="onoffswitch" id="lampe1">
    <input type="checkbox" name="onoffswitch" class="onoffswitch-checkbox" id="lampe_bureau" value="<?php if($etat_lampe0['etat_lampe'] =="1"){
    	echo "1";
    }
    elseif($etat_lampe0['etat_lampe'] == "0"){
    	echo "0";
    }

    ?>" <?php if($etat_lampe0['etat_lampe'] =="1"){
    	echo "checked";
    }
    	 ?> onclick="actionLampe(0)">
    <label class="onoffswitch-label" for="lampe_bureau"></label>
</div>
</div>
<div class="lampe_block">
<h1>Lampe lit</h1>
<div class="onoffswitch" id="lampe2">
    <input type="checkbox" name="onoffswitch" class="onoffswitch-checkbox" id="lampe_lit" value="<?php if($etat_lampe1['etat_lampe'] =="1"){
    	echo "1";
    }
    elseif($etat_lampe1['etat_lampe'] == "0"){
    	echo "0";
    }

    ?>" <?php if($etat_lampe1['etat_lampe'] =="1"){
    	echo "checked";
    }
    	 ?> onclick="actionLampe(1)">
    <label class="onoffswitch-label" for="lampe_lit"></label>
</div>
</div>
<div class="lampe_block">
<h1>Lampe 3</h1>
<div class="onoffswitch" id="lampe3">
    <input type="checkbox" name="onoffswitch" class="onoffswitch-checkbox" id="lampe_3" value="<?php if($etat_lampe2['etat_lampe'] =="1"){
    	echo "1";
    }
    elseif($etat_lampe2['etat_lampe'] == "0"){
    	echo "0";
    }

    ?>" <?php if($etat_lampe2['etat_lampe'] =="1"){
    	echo "checked";
    }
    	 ?> onclick="actionLampe(2)">
    <label class="onoffswitch-label" for="lampe_3" id="label_lampe_3"></label>
</div>
</div>
<button onclick="actionGroupe(1)">On Groupe</button>
<button onclick="actionGroupe(0)">Off Groupe</button>
</div>
<div class="volets">
<img src="up.png" onclick="actionVolet('R', 'M')" id="telcommande"><br />
<img src="middle.png" onclick="actionVolet('R', 'S')" id="telcommande"><br />
<img src="down.png" onclick="actionVolet('R', 'B')" id="telcommande">
</div>
</div>
</body>
</html>
