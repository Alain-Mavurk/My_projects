<?php


$title="Action enseignant";
include("header.php");
require("modal.php");

$SQL = "SELECT users.uid FROM users
    JOIN enseignants on users.uid = enseignants.uid
    WHERE enseignants.annee = :annee
    GROUP BY uid
    ORDER BY uid DESC";
$res = $db->prepare($SQL);
$res->execute(array(
    'annee' => htmlspecialchars($_SESSION['annee'])
));

$SQL = "SELECT eid, uid, e.nom, prenom, email, tel, etypes.nom statut FROM enseignants e
    JOIN etypes on e.etid = etypes.etid
    WHERE annee = :annee
";
$stmt = $db->prepare($SQL);
$stmt->execute(array(
    'annee' => htmlspecialchars($_SESSION['annee'])
));



if(isset($_POST['modif'])){ ?>
    <table class ="table table-hover">
        <thead class="black white-text">
            <tr>
                <th>Uid</th>
                <th>Nom</th>
                <th>Prenom</th>
                <th>Email</th>
                <th>Tel</th>
                <th>Statut</th>
            </tr>
        </thead>
        <tbody>
            <?php while($donnee = $stmt->fetch()){ 

                if(!strcmp($_POST['eid'], $donnee['eid'])){ ?>
                    <form method="post">
                        <tr>
                            <td>
                                <select name="uid" class="form-control">
                                    <?php while($data = $res->fetch()){ 
                                        if(strcmp($_POST['uid'], $data['uid'])){ ?>
                                            <option value="<?php echo $data['uid']?>"><?php echo htmlspecialchars($data['uid'])?></option>
                                    <?php }else{ ?>
                                        <option value="<?php echo $data['uid']?>" selected="selected"><?php echo htmlspecialchars($data['uid'])?></option>
                                    <?php }
                                    }  ?>
                                </select>
                            </td>
                            <td><input type="text" class="form-control" name="nom" maxlength="30" value="<?php echo htmlspecialchars($_POST['nom'])?>"></td>
                            <td><input type="text" class="form-control" name="prenom" maxlength="30" value="<?php echo htmlspecialchars($_POST['prenom'])?>"></td>
                            <td><input type="email" class="form-control" name="email" maxlength="30" value="<?php echo htmlspecialchars($_POST['email'])?>"></td>
                            <td><input type="tel" class="form-control" name="tel" maxlength="15" value="<?php echo htmlspecialchars($_POST['tel'])?>"></td>
                            <td><input type="text" class="form-control" name="statut" value="<?php echo htmlspecialchars($_POST['statut'])?>" pattern="ATER|VAC1|MCF|PR" title="ATER, VAC1, MCF ou PR"></td>
                            <td><input type="hidden" name="eid" value="<?php echo $_POST['eid']?>"></td>
                            <td><button type="button" class="btn" data-toggle="modal" data-target="#<?php echo $donnee['eid']?>"><span class="glyphicon glyphicon-ok" style="color:green;"></span></button></td>
                            <td><button class="btn"><a href="CRUDenseignant.php"><span class="glyphicon glyphicon-ban-circle" style="color:red;"></span></a></button></td>
                            <?php modifEnseignant($donnee['eid'], $donnee['nom'], $donnee['prenom']); ?>
                        </tr>
                    </form>
               <?php }else{ ?>
                    <tr>
                        <td><?php echo htmlspecialchars($donnee['uid'])?></td>
                        <td><?php echo htmlspecialchars($donnee['nom'])?></td>
                        <td><?php echo htmlspecialchars($donnee['prenom'])?></td>
                        <td><?php echo htmlspecialchars($donnee['email'])?></td>
                        <td><?php echo htmlspecialchars($donnee['tel'])?></td>
                        <td><?php echo htmlspecialchars($donnee['statut'])?></td>
                        <td></td>
                        <td></td>
                        <td></td>
                    </tr>
               <?php }
                } ?>
        </tbody>
    </table>

<?php    

}else{
?>

<p class="error"><?= $error??""?></p>

<table class ="table table-hover">
    <thead class="black white-text">
        <tr>
            <th>Uid</th>
            <th>Nom</th>
            <th>Prenom</th>
            <th>Email</th>
            <th>Tel</th>
            <th>Statut</th>
        </tr>
    </thead>
    <tbody>
        <?php while($donnee = $stmt->fetch()){ 
            supEnseignant($donnee['eid'], $donnee['nom'], $donnee['prenom']);
            ?>
        <tr>
            <td><?php echo htmlspecialchars($donnee['uid'])?></td>
            <td><?php echo htmlspecialchars($donnee['nom'])?></td>
            <td><?php echo htmlspecialchars($donnee['prenom'])?></td>
            <td><?php echo htmlspecialchars($donnee['email'])?></td>
            <td><?php echo htmlspecialchars($donnee['tel'])?></td>
            <td><?php echo htmlspecialchars($donnee['statut'])?></td>
            <td>
                <form method="post">
                    <input type="hidden" name="eid" value="<?php echo htmlspecialchars($donnee['eid'])?>" >
                    <input type="hidden" name="uid" value="<?php echo htmlspecialchars($donnee['uid'])?>" >
                    <input type="hidden" name="nom" maxlength="30" value="<?php echo htmlspecialchars($donnee['nom'])?>" >
                    <input type="hidden" name="prenom" maxlength="30" value="<?php echo htmlspecialchars($donnee['prenom'])?>" >
                    <input type="hidden" name="email" maxlength="30" value="<?php echo htmlspecialchars($donnee['email'])?>" >
                    <input type="hidden" name="tel" maxlength="15" value="<?php echo htmlspecialchars($donnee['tel'])?>" >
                    <input type="hidden" name="statut" value="<?php echo htmlspecialchars($donnee['statut'])?>" >
                    <button type="submit" name="modif" class="btn"><span class="glyphicon glyphicon-edit" style="color:rgb(30,144,255);"></span></button>
                </form>
            </td>
            <td><button type="submit" class="btn" data-toggle="modal" data-target="#<?php echo $donnee['eid']?>"><span class="glyphicon glyphicon-remove" style="color:red;"></span></button></td>
        </tr>
        <?php } ?>
            <form method="post">
                <tr>
                    <td>
                        <select name="uid" class="form-control">
                        <?php while($donnee = $res->fetch()){ ?>
                            <option value="<?php echo $donnee['uid']?>"><?php echo htmlspecialchars($donnee['uid'])?></option>
                        <?php } ?>
                        </select>
                    </td>
                    <td><input type="text" class="form-control" name="nom" maxlength="30" placeholder="Jean"></td>
                    <td><input type="text" class="form-control" name="prenom" maxlength="30" placeholder="Michel"></td>
                    <td><input type="email" class="form-control" name="email" maxlength="30" placeholder="jmichel@mail.com"></td>
                    <td><input type="tel" class="form-control" name="tel" maxlength="15" placeholder="01002540325"></td>
                    <td><input type="text" class="form-control" name="statut" placeholder="ATER" pattern="ATER|VAC1|MCF|PR" title="ATER, VAC1, MCF ou PR"></td>
                    <td><button type="submit" name="ajout" class="btn"><span class="glyphicon glyphicon-plus" style="color:green;"></span></button></td>
                    <td></td>
                </tr>
            </form>
    </tbody>
</table>

<?php
}

include("footer.php");
?>