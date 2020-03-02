<?php

$authTableData = [
    'table' => 'users',
    'idfield' => 'login',
    'cfield' => 'mdp',
    'uidfield' => 'uid',
    'rfield' => 'role',
];

$pathFor = [
    "login"  => "/login.php",
    "logout" => "/logout.php",
    "adduser" => "/adduser.php",
    "root"   => "/",
    "TBenseignant" => "/TBenseignant.php",
    "home" => "/home.php",
    "mdp" => "/mdp.php",
    "TBgroupes" => "/TBgroupes.php",
    "TBmodules" => "/TBmodules.php",
    "CRUDuser" => "/CRUDuser.php",
    "CRUDenseignant" => "/CRUDenseignant.php",
    "CRUDmodule" => "/CRUDmodule.php",
    "CRUDgroupe" => "/CRUDgroupe.php"
];

const SKEY = '_Redirect';