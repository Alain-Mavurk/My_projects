<?php

require("auth/EtreAuthentifie.php");


if(!isset($_POST['sup']) && !isset($_POST['ajout']) && !isset($_POST['modif2'])){
    include("CRUDenseignant_form.php");
    exit();
}

if(isset($_POST['sup'])){
    try {
        $SQL="DELETE FROM enseignants WHERE eid = :eid";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'eid' => htmlspecialchars($_POST['eid'])
        ));
        redirect($pathFor['CRUDenseignant.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}

if(isset($_POST['modif2'])){
    $error = "";
    
    foreach (['uid','nom', 'prenom', 'email', 'tel', 'statut'] as $name) {
        if (empty($_POST[$name])) {
            $error .= "La valeur du champs '$name' ne doit pas être vide. ";
        }
    }    

    if (!empty($error)) {
        include('CRUDenseignant_form.php');
        exit();
    }

    try {
        $SQL = "SELECT etid FROM etypes WHERE nom = :nom";
        $stmt = $db->prepare($SQL);
        $stmt->execute(array(
            'nom' => htmlspecialchars($_POST['statut'])
        ));
        $donnee= $stmt->fetch();
        $etid = $donnee['etid'];

        $SQL="UPDATE enseignants SET uid = :uid, nom = :nom, prenom= :prenom, email= :email, tel = :tel, etid = :etid WHERE eid = :eid";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'uid' => htmlspecialchars($_POST['uid']),
            'nom' => htmlspecialchars($_POST['nom']),
            'prenom' => htmlspecialchars($_POST['prenom']),
            'email' => htmlspecialchars($_POST['email']),
            'tel' => htmlspecialchars($_POST['tel']),
            'etid' => htmlspecialchars($etid),
            'eid' => htmlspecialchars($_POST['eid'])
        ));
        redirect($pathFor['CRUDenseignant.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}


if(isset($_POST['ajout'])){
    $error = "";
    
    foreach (['nom', 'prenom', 'email', 'tel', 'statut'] as $name) {
        if (empty($_POST[$name])) {
            $error .= "La valeur du champs '$name' ne doit pas être vide. ";
        }
    }  

    if (!empty($error)) {
        include('CRUDenseignant_form.php');
        exit();
    }

    try {
        
        $SQL = "SELECT etid FROM etypes WHERE nom = :nom";
        $stmt = $db->prepare($SQL);
        $stmt->execute(array(
            'nom' => htmlspecialchars($_POST['statut'])
        ));
        $donnee= $stmt->fetch();
        $etid = $donnee['etid'];

        $SQL="INSERT INTO enseignants(uid, nom, prenom, email, tel, etid, annee) VALUES(:uid, :nom, :prenom, :email, :tel, :etid, :annee)";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'uid' => htmlspecialchars($_POST['uid']),
            'nom' => htmlspecialchars($_POST['nom']),
            'prenom' => htmlspecialchars($_POST['prenom']),
            'email' => htmlspecialchars($_POST['email']),
            'tel' => htmlspecialchars($_POST['tel']),
            'etid' => htmlspecialchars($etid),
            'annee' => htmlspecialchars($_SESSION['annee'])

        ));
        redirect($pathFor['CRUDenseignant.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}