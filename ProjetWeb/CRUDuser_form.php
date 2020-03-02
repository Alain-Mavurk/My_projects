<?php


$title="Action utilisateur";
include("header.php");
require("modal.php");

$SQL = "SELECT uid, login, role FROM users";
$stmt = $db->prepare($SQL);
$stmt->execute();


if(isset($_POST['modif'])){ ?>
    <table class ="table table-hover">
        <thead class="black white-text">
            <tr>
                <th>Login</th>
                <th>Role</th>
            </tr>
        </thead>
        <tbody>
            <?php while($donnee = $stmt->fetch()){ 

                if(!strcmp($_POST['uid'], $donnee['uid'])){ ?>
                    <form method="post">
                        <tr>
                            <input type="hidden" name="uid" value="<?php echo htmlspecialchars($donnee['uid'])?>">
                            <td><input type="text" class="form-control" name="login" maxlength="30" value="<?php echo htmlspecialchars($_POST['login'])?>"> </td>
                            <td><input type="text" class="form-control" name="role" value="<?php echo htmlspecialchars($_POST['role'])?>" pattern="user|admin" title="admin ou user"> </td>
                            <td><input type="password" class="form-control" name="mdp" placeholder="mdp"></td>
                            <td><input type="password" class="form-control" name="mdp2" placeholder="Répéter mdp"></td>
                            <td><button type="button" class="btn" data-toggle="modal" data-target="#<?php echo $donnee['uid']?>"><span class="glyphicon glyphicon-ok" style="color:green;"></span></button></td>
                            <td><button class="btn"><a href="CRUDuser.php"><span class="glyphicon glyphicon-ban-circle" style="color:red;"></span></a></button></td>
                            <?php modif($donnee['uid'], $donnee['login']); ?>
                        </tr>
                    </form>
               <?php }else{ ?>
                    <tr>
                        <td><?php echo htmlspecialchars($donnee['login'])?></td>
                        <td><?php echo htmlspecialchars($donnee['role'])?></td>
                        <td></td>
                        <td></td>
                        <td></td>
                        <td></td>
                        <td></td>
                    </tr>
               <?php }
                } ?>
        </tbody>
    </table>

<?php    

       
}else if(isset($_POST['affect'])){
    $SQL = "SELECT eid, nom, prenom FROM enseignants WHERE annee = :annee";
    $stmt = $db->prepare($SQL);
    $stmt->execute(array(
        'annee' => htmlspecialchars($_SESSION['annee'])
    ));

    ?>
    <table class ="table table-hover">
    <thead class="black white-text">
        <tr>
            <th>Nom</th>
            <th>Prenom</th>
        </tr>
    </thead>
    <tbody>
        <?php while($donnee = $stmt->fetch()){ ?>
            <tr>
                <td><?php echo htmlspecialchars($donnee['nom'])?></td>
                <td><?php echo htmlspecialchars($donnee['prenom'])?></td>
                <td>
                    <form method="post">
                        <input type="hidden" name="uid" value="<?php echo htmlspecialchars($_POST['uid'])?>" >
                        <input type="hidden" name="eid" value="<?php echo htmlspecialchars($donnee['eid'])?>" >
                        <button type="button" class="btn" data-toggle="modal" data-target="#<?php echo $donnee['eid']?>"><span class="glyphicon glyphicon-ok-sign" style="color:green;"></span></button>
                        <button class="btn"><a href="CRUDuser.php"><span class="glyphicon glyphicon-ban-circle" style="color:red;"></span></a></button>
                        <?php affect($donnee['eid'], $donnee['nom'], $donnee['prenom'], $_POST['login']); ?>
                    </form>
                </td>
            </tr>
        <?php } ?>
    </tbody>
    </table>

<?php

}else{
?>

<p class="error"><?= $error??""?></p>

<table class ="table table-hover">
    <thead class="black white-text">
        <tr>
            <th>Login</th>
            <th>Role</th>
        </tr>
    </thead>
    <tbody>
        <?php while($donnee = $stmt->fetch()){ 
            sup($donnee['uid'], $donnee['login']);
            ?>
        <tr>
            <td><?php echo htmlspecialchars($donnee['login'])?></td>
            <td><?php echo htmlspecialchars($donnee['role'])?></td>
            <td></td>
            <td></td>
            <td>
                <form method="post">
                    <input type="hidden" name="uid" value="<?php echo htmlspecialchars($donnee['uid'])?>" >
                    <input type="hidden" name="login" value="<?php echo htmlspecialchars($donnee['login'])?>" >
                    <input type="hidden" name="role" value="<?php echo htmlspecialchars($donnee['role'])?>" >
                    <button type="submit" name="modif" class="btn"><span class="glyphicon glyphicon-edit" style="color:rgb(30,144,255);"></span></button>
                </form>
            </td>
            <td>
                <form method="post">
                    <input type="hidden" name="uid" value="<?php echo htmlspecialchars($donnee['uid'])?>" >
                    <input type="hidden" name="login" value="<?php echo htmlspecialchars($donnee['login'])?>" >
                    <input type="hidden" name="role" value="<?php echo htmlspecialchars($donnee['role'])?>" >
                    
                    <button type="submit" name="affect" class="btn"><span class="glyphicon glyphicon-plus-sign" style="color:darkorange;"></span></button>
                    
                </form>
            </td>
            <?php if(strcmp($idm->getUid(), $donnee['uid'])){ ?>
            <td><button type="submit" class="btn" data-toggle="modal" data-target="#<?php echo $donnee['uid']?>"><span class="glyphicon glyphicon-remove" style="color:red;"></span></button></td>
        <?php }else{ ?>
            <td></td>
       <?php } ?>
            </tr>
        <?php } ?>
            <form method="post">
                <tr>
                    <td><input type="text" class="form-control" name="login" placeholder="jmichel"></td>
                    <td><input type="text" class="form-control" name="role" placeholder="user" pattern="user|admin" title="admin ou user"></td>
                    <td><input type="password" class="form-control" name="mdp" placeholder="mdp"></td>
                    <td><input type="password" class="form-control" name="mdp2" placeholder="Répéter mdp"></td>
                    <td><button type="submit" name="ajout" class="btn"><span class="glyphicon glyphicon-plus" style="color:green;"></span></button></td>
                </tr>
            </form>
    </tbody>
</table>

<?php
}

include("footer.php");
?>