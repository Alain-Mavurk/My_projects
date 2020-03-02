<?php

require("auth/EtreAuthentifie.php");

if(empty($_POST['mdp'])){
    include("mdp_form.php");
    exit();
}

$error = "";

foreach (['mdp', 'mdp2'] as $name) {
    if (empty($_POST[$name])) {
        $error .= "La valeur du champs '$name' ne doit pas être vide";
    } else {
        $data[$name] = $_POST[$name];
    }
}


if ($data['mdp'] != $data['mdp2']) {
    $error .="MDP ne correspondent pas";
}

if (!empty($error)) {
    include('mdp_form.php');
    exit();
}

$clearData['mdp'] = $data['mdp'];

$passwordFunction =
    function ($s) {
        return password_hash($s, PASSWORD_DEFAULT);
    };

$clearData['mdp'] = $passwordFunction($data['mdp']);

try {
    $SQL = "UPDATE users SET mdp = :mdp WHERE uid = :uid";
    $stmt = $db->prepare($SQL);
    $res = $stmt->execute(array(
        'mdp' => $clearData['mdp'],
        'uid' => $idm->getUid()
    ));
    // echo "Utilisateur $clearData[nom] : " . $id . " ajouté avec succès.";
    redirect($pathFor['root']);
} catch (\PDOException $e) {
    http_response_code(500);
    echo "Erreur de serveur.";
    exit();
}


?>
