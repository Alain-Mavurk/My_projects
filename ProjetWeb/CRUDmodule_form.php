<?php


$title="Action module";
include("header.php");
require("modal.php");

$SQL = "SELECT nom, cid FROM categories";
$cid_res = $db->prepare($SQL);
$cid_res->execute();

$SQL = "SELECT eid, nom, prenom FROM enseignants
    WHERE annee= :annee
";
$eid_res = $db->prepare($SQL);
$eid_res->execute(array(
    'annee' => htmlspecialchars($_SESSION['annee'])
));

$SQL = "SELECT mid, intitule, code, e.eid, e.nom nomProf, e.prenom, c.nom, c.cid FROM modules m
    JOIN categories c on c.cid = m.cid
    JOIN enseignants e on e.eid = m.eid
    WHERE m.annee = :annee
";
$stmt = $db->prepare($SQL);
$stmt->execute(array(
    'annee' => htmlspecialchars($_SESSION['annee'])
));



if(isset($_POST['modif'])){ ?>
    <table class ="table table-hover">
        <thead class="black white-text">
            <tr>
                <th>Module</th>
                <th>Code</th>
                <th>Enseignants</th>
                <th>Cursus</th>
            </tr>
        </thead>
        <tbody>
            <?php while($donnee = $stmt->fetch()){ 

                if(!strcmp($_POST['mid'], $donnee['mid'])){ ?>
                    <form method="post">
                        <tr>
                            <td><input type="text" class="form-control" name="intitule" maxlength="30" value="<?php echo htmlspecialchars($_POST['intitule'])?>"></td>
                            <td><input type="text" class="form-control" name="code" maxlength="10" value="<?php echo htmlspecialchars($_POST['code'])?>"></td>
                            <td>
                                <select name="eid" class="form-control">
                                    <?php while($data = $eid_res->fetch()){
                                        if(strcmp($_POST['eid'], $data['eid'])){
                                        ?>
                                            <option value="<?php echo $data['eid']?>"><?php echo htmlspecialchars($data['nom']) . " " . htmlspecialchars($data['prenom'])?></option>
                                 <?php }else{ ?>
                                            <option value="<?php echo $data['eid']?>" selected="selected"><?php echo htmlspecialchars($data['nom']) . " " . htmlspecialchars($data['prenom'])?></option>
                                    <?php } 
                                    } ?>
                                </select>
                            </td>
                            <td>
                                <select name="cid" class="form-control">
                                    <?php while($data = $cid_res->fetch()){ 
                                        if(strcmp($_POST['cid'], $data['cid'])){
                                        ?>
                                        <option value="<?php echo $data['cid']?>"><?php echo htmlspecialchars($data['nom'])?></option>
                                <?php }else{ ?>
                                        <option value="<?php echo $data['cid']?>" selected="selected"><?php echo htmlspecialchars($data['nom'])?></option>
                               <?php }
                               } ?>
                                </select>
                            </td>
                            <td><input type="hidden" name="mid" value="<?php echo $_POST['mid']?>" ></td>
                            <td><button type="button" class="btn" data-toggle="modal" data-target="#<?php echo $donnee['mid']?>"><span class="glyphicon glyphicon-ok" style="color:green;"></span></button></td>
                            <td><button class="btn"><a href="CRUDmodule.php"><span class="glyphicon glyphicon-ban-circle" style="color:red;"></span></a></button></td>
                            <?php modifModule($donnee['mid'], $donnee['nom']); ?>
                        </tr>
                    </form>
               <?php }else{ ?>
                    <tr>
                        <td><?php echo htmlspecialchars($donnee['intitule'])?></td>
                        <td><?php echo htmlspecialchars($donnee['code'])?></td>
                        <td><?php echo htmlspecialchars($donnee['nomProf']) . " " . htmlspecialchars($donnee['prenom'])?></td>
                        <td><?php echo htmlspecialchars($donnee['nom'])?></td>
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
            <th>Module</th>
            <th>Code</th>
            <th>Enseignants</th>
            <th>Cursus</th>
        </tr>
    </thead>
    <tbody>
        <?php while($donnee = $stmt->fetch()){ 
            supModule($donnee['mid'], $donnee['intitule']);
            ?>
        <tr>
            <td><?php echo htmlspecialchars($donnee['intitule'])?></td>
            <td><?php echo htmlspecialchars($donnee['code'])?></td>
            <td><?php echo htmlspecialchars($donnee['nomProf']) . " " . htmlspecialchars($donnee['prenom'])?></td>
            <td><?php echo htmlspecialchars($donnee['nom'])?></td>
            <td>
                <form method="post">
                    <input type="hidden" name="mid" value="<?php echo htmlspecialchars($donnee['mid'])?>" >
                    <input type="hidden" name="intitule" value="<?php echo htmlspecialchars($donnee['intitule'])?>" >
                    <input type="hidden" name="code" value="<?php echo htmlspecialchars($donnee['code'])?>" >
                    <input type="hidden" name="eid" value="<?php echo htmlspecialchars($donnee['eid'])?>" >
                    <input type="hidden" name="cid" value="<?php echo htmlspecialchars($donnee['cid'])?>" >
                    <button type="submit" name="modif" class="btn"><span class="glyphicon glyphicon-edit" style="color:rgb(30,144,255);"></span></button>
                </form>
            </td>
            <td><button type="submit" class="btn" data-toggle="modal" data-target="#<?php echo $donnee['mid']?>"><span class="glyphicon glyphicon-remove" style="color:red;"></span></button></td>
        </tr>
        <?php } ?>
            <form method="post">
                <tr>
                    <td><input type="text" class="form-control" name="intitule" maxlength="30" placeholder="Compilation"></td>
                    <td><input type="text" class="form-control" name="code" maxlength="10" placeholder="X65E4DF"></td>
                    <td>
                        <select name="eid" class="form-control">
                            <?php while($data = $eid_res->fetch()){ ?>
                                <option value="<?php echo $data['eid']?>"><?php echo htmlspecialchars($data['nom']) . " " . htmlspecialchars($data['prenom'])?></option>
                           <?php } ?>
                        </select>
                    </td>
                    <td>
                        <select name="cid" class="form-control">
                            <?php while($data = $cid_res->fetch()){ ?>
                                <option value="<?php echo $data['cid']?>"><?php echo htmlspecialchars($data['nom'])?></option>
                           <?php } ?>
                        </select>
                    </td>
                    <td><button type="submit" name="ajout" class="btn"><span class="glyphicon glyphicon-plus" style="color:green;"></span></button></td>
                </tr>
            </form>
    </tbody>
</table>

<?php
}
include("footer.php");
?>