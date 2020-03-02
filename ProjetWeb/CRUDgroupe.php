<?php

require("auth/EtreAuthentifie.php");


if(!isset($_POST['sup']) && !isset($_POST['ajout']) && !isset($_POST['modif2'])){
    include("CRUDgroupe_form.php");
    exit();
}

if(isset($_POST['sup'])){
    try {
        $SQL="DELETE FROM groupes WHERE gid = :gid";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'gid' => htmlspecialchars($_POST['gid'])
        ));
        redirect($pathFor['CRUDgroupe.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}

if(isset($_POST['modif2'])){
    $error = "";
    
    foreach (['nom', 'mid', 'gtid'] as $name) {
        if (empty($_POST[$name])) {
            $error .= "La valeur du champs '$name' ne doit pas être vide. ";
        }
    }    

    if (!empty($error)) {
        include('CRUDgroupe_form.php');
        exit();
    }

    try {

        $SQL="UPDATE groupes SET nom = :nom, mid = :mid, gtid = :gtid WHERE gid = :gid";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'nom' => htmlspecialchars($_POST['nom']),
            'mid' => htmlspecialchars($_POST['mid']),
            'gtid' => htmlspecialchars($_POST['gtid']),
            'gid' => htmlspecialchars($_POST['gid'])
        ));
        redirect($pathFor['CRUDgroupe.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }

}


if(isset($_POST['ajout'])){
    $error = "";
    
    foreach (['nom', 'mid', 'gtid'] as $name) {
        if (empty($_POST[$name])) {
            $error .= "La valeur du champs '$name' ne doit pas être vide. ";
        }else{
            echo $_POST[$name];
        }
    }  

    if (!empty($error)) {
        include('CRUDgroupe_form.php');
        exit();
    }

    try {

        $SQL="INSERT INTO groupes(nom, mid, gtid, annee) VALUES(:nom, :mid, :gtid, :annee)";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'nom' => htmlspecialchars($_POST['nom']),
            'mid' => htmlspecialchars($_POST['mid']),
            'gtid' => htmlspecialchars($_POST['gtid']),
            'annee' => htmlspecialchars($_SESSION['annee'])
        ));
        redirect($pathFor['CRUDgroupe.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}