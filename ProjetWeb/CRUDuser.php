<?php

require("auth/EtreAuthentifie.php");

$passwordFunction =
    function ($s) {
        return password_hash($s, PASSWORD_DEFAULT);
};

if(!isset($_POST['sup']) && !isset($_POST['ajout']) && !isset($_POST['modif2']) && !isset($_POST['affect2'])){
    include("CRUDuser_form.php");
    exit();
}

if(isset($_POST['sup'])){
    try {
        $SQL="DELETE FROM users WHERE uid = :uid";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'uid' => $_POST['uid']
        ));
        redirect($pathFor['CRUDuser.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}

if(isset($_POST['modif2'])){
    $error = "";
    
    foreach (['login', 'role'] as $name) {
        if (empty($_POST[$name])) {
            $error .= "La valeur du champs '$name' ne doit pas être vide. ";
        }
    }   

    if ($_POST['mdp'] != $_POST['mdp2']) {
        $error .="MDP ne correspondent pas. ";
    }

    if (!empty($error)) {
        include('CRUDuser_form.php');
        exit();
    }
    $clearData['mdp'] = $passwordFunction(htmlspecialchars($_POST['mdp']));
    try {
        $SQL="UPDATE users SET login = :login, role= :role, mdp= :mdp WHERE uid = :uid";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'login' => htmlspecialchars($_POST['login']),
            'role' => htmlspecialchars($_POST['role']),
            'mdp' => $clearData['mdp'],
            'uid' => htmlspecialchars($_POST['uid'])
        ));
        redirect($pathFor['CRUDuser.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}

if(isset($_POST['affect2'])){
    try {
        $SQL="UPDATE enseignants SET uid = :uid WHERE eid = :eid";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'uid' => htmlspecialchars($_POST['uid']),
            'eid' => htmlspecialchars($_POST['eid']),
        ));
        redirect($pathFor['CRUDuser.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}

if(isset($_POST['ajout'])){
    $error = "";
    
    foreach (['login', 'role', 'mdp', 'mdp2'] as $name) {
        if (empty($_POST[$name])) {
            $error .= "La valeur du champs '$name' ne doit pas être vide. ";
        }
    } 

    if ($_POST['mdp'] != $_POST['mdp2']) {
        $error .="MDP ne correspondent pas. ";
    }

    if (!empty($error)) {
        include('CRUDuser_form.php');
        exit();
    }
    $clearData['mdp'] = $passwordFunction(htmlspecialchars($_POST['mdp']));
    try {
        $SQL="INSERT INTO users(login, role, mdp) VALUES(:login, :role, :mdp)";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'login' => htmlspecialchars($_POST['login']),
            'role' => htmlspecialchars($_POST['role']),
            'mdp' => $clearData['mdp']
        ));
        redirect($pathFor['CRUDuser.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}