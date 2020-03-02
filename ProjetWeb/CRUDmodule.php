<?php

require("auth/EtreAuthentifie.php");


if(!isset($_POST['sup']) && !isset($_POST['ajout']) && !isset($_POST['modif2'])){
    include("CRUDmodule_form.php");
    exit();
}

if(isset($_POST['sup'])){
    try {
        $SQL="DELETE FROM modules WHERE mid = :mid";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'mid' => htmlspecialchars($_POST['mid'])
        ));
        redirect($pathFor['CRUDmodule.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}

if(isset($_POST['modif2'])){
    $error = "";
    
    foreach (['intitule', 'code', 'eid', 'cid'] as $name) {
        if (empty($_POST[$name])) {
            $error .= "La valeur du champs '$name' ne doit pas être vide. ";
        }
    }    

    if (!empty($error)) {
        include('CRUDmodule_form.php');
        exit();
    }

    try {

        $SQL="UPDATE modules SET intitule = :intitule, code = :code, eid= :eid, cid= :cid WHERE mid = :mid";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'intitule' => htmlspecialchars($_POST['intitule']),
            'code' => htmlspecialchars($_POST['code']),
            'eid' => htmlspecialchars($_POST['eid']),
            'cid' => htmlspecialchars($_POST['cid']),
            'mid' => htmlspecialchars($_POST['mid'])
        ));
        redirect($pathFor['CRUDmodule.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }

}


if(isset($_POST['ajout'])){
    $error = "";
    
    foreach (['intitule', 'code', 'eid', 'cid'] as $name) {
        if (empty($_POST[$name])) {
            $error .= "La valeur du champs '$name' ne doit pas être vide. ";
        }
    }  

    if (!empty($error)) {
        include('CRUDmodule_form.php');
        exit();
    }

    try {

        $SQL="INSERT INTO modules(intitule, code, eid, cid, annee) VALUES(:intitule, :code, :eid, :cid, :annee)";
        $stmt = $db->prepare($SQL);
        $res = $stmt->execute(array(
            'intitule' => htmlspecialchars($_POST['intitule']),
            'code' => htmlspecialchars($_POST['code']),
            'eid' => htmlspecialchars($_POST['eid']),
            'cid' => htmlspecialchars($_POST['cid']),
            'annee' => htmlspecialchars($_SESSION['annee'])
        ));
        redirect($pathFor['CRUDmodule.php']);
    } catch (\PDOException $e) {
        http_response_code(500);
        echo "Erreur de serveur.";
        exit();
    }
}