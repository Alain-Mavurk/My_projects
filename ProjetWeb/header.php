<!DOCTYPE html>

<html>

<head>
<meta charset="utf-8"/>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>



<title><?= $title ?></title>

    <style>
        .center { text-align: center }
        .center table {margin-left:auto; margin-right:auto;}
        .left {text-align: right}
        .right {text-align: left}
        body {background-color: white}
        .error {color: red;}
        table{
            width: 50%;
        }
    </style>
    

</head>

<body>

<nav class="navbar navbar-inverse">
  <div class="container-fluid">
    <div class="navbar-header">
      <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#myNavbar">
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
      <a class="navbar-brand" href="<?= $pathFor['home'] ?>">Gestion</a>
    </div>
    <div class="collapse navbar-collapse" id="myNavbar">
      <ul class="nav navbar-nav">
        <li class="active"><a href="<?= $pathFor['home'] ?>"><span class="glyphicon glyphicon-home"></span> Home</a></li>
        <?php if($idm->getRole()){ ?>
          <?php if(!strcmp($idm->getRole(), "admin")){ ?>
          <li><a href="<?= $pathFor['CRUDuser'] ?>">Utilisateurs</a></li>
          <?php } ?>
          <li class="dropdown">
          <a class="dropdown-toggle" data-toggle="dropdown" href="#">Enseignants
          <span class="caret"></span></a>
          <ul class="dropdown-menu">
            <li><a href="<?= $pathFor['TBenseignant'] ?>">Tableau de bord</a></li>
            <?php if(!strcmp($idm->getRole(), "admin")){ ?>
            <li><a href="<?= $pathFor['CRUDenseignant'] ?>">Liste</a></li>
            <?php }?>
          </ul>
        </li>
          <?php if(!strcmp($idm->getRole(), "admin")){ ?>
          <li class="dropdown">
            <a class="dropdown-toggle" data-toggle="dropdown" href="#">Modules
            <span class="caret"></span></a>
            <ul class="dropdown-menu">
              <li><a href="<?= $pathFor['TBmodules'] ?>">Tableau de bord</a></li>
              <li><a href="<?= $pathFor['CRUDmodule'] ?>">Liste</a></li>
            </ul>
          </li>
          <li class="dropdown">
            <a class="dropdown-toggle" data-toggle="dropdown" href="#">Groupe
            <span class="caret"></span></a>
            <ul class="dropdown-menu">
              <li><a href="<?= $pathFor['TBgroupes'] ?>">Tableau de bord</a></li>
              <li><a href="<?= $pathFor['CRUDgroupe'] ?>">Liste</a></li>
            </ul>
          </li>
          <?php } ?>
          <li><a href="<?= $pathFor['mdp'] ?>">Changer mdp</a></li>
        <?php } ?>
      </ul>
      <ul class="nav navbar-nav navbar-right">
        <?php if(!$idm->getRole()){?>
        <li><a href="<?= $pathFor['login'] ?>"><span class="glyphicon glyphicon-log-in"></span> Login</a></li>
        <?php }?>
        <?php if($idm->getRole()){?>
        <li><a href="<?= $pathFor['logout'] ?>"><span class="glyphicon glyphicon-log-out"></span> Logout</a></li>
        <?php }?>
      </ul>
    </div>
  </div>
</nav> 

<?php

if(!isset($_SESSION['annee']))
    $_SESSION['annee'] = date("Y");

if(isset($_POST['annee']))
    $_SESSION['annee'] = htmlspecialchars($_POST['annee']);
 
?>    