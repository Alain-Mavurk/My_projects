<?php


$title="Action groupe";
include("header.php");
require("modal.php");



$SQL = "SELECT intitule, mid FROM modules 
GROUP BY intitule, mid";
$mid_res = $db->prepare($SQL);
$mid_res->execute();

$SQL = "SELECT gtid, nom FROM gtypes";
$gtid_res = $db->prepare($SQL);
$gtid_res->execute();

$SQL = "SELECT gid, nom FROM groupes GROUP BY nom, gid";
$gid_res = $db->prepare($SQL);
$gid_res->execute();

$SQL = "SELECT gid, groupes.nom, groupes.mid, groupes.gtid, gtypes.nom gnom, intitule FROM groupes
    JOIN modules m on m.mid = groupes.mid
    JOIN gtypes on gtypes.gtid = groupes.gtid
    WHERE groupes.annee= :annee
";

$stmt = $db->prepare($SQL);
$stmt->execute(array(
    'annee' => htmlspecialchars($_SESSION['annee'])
));



if(isset($_POST['modif'])){ ?>
    <table class ="table table-hover">
        <thead class="black white-text">
            <tr>
                <th>Groupe</th>
                <th>Module</th>
                <th>gtid</th>
            </tr>
        </thead>
        <tbody>
            <?php while($donnee = $stmt->fetch()){ 

                if(!strcmp($_POST['gid'], $donnee['gid'])){ ?>
                    <form method="post">
                        <tr>
                            <td>
                                <select name="nom" class="form-control">
                                    <?php while($data = $gid_res->fetch()){
                                        if(strcmp($_POST['nom'], $data['nom'])){
                                        ?>
                                            <option value="<?php echo $data['nom']?>"><?php echo htmlspecialchars($data['nom'])?></option>
                                 <?php }else{ ?>
                                            <option value="<?php echo $data['nom']?>" selected="selected"><?php echo htmlspecialchars($data['nom'])?></option>
                                    <?php } 
                                    } ?>
                                </select>
                            </td>
                            <td>
                                <select name="mid" class="form-control">
                                    <?php while($data = $mid_res->fetch()){
                                        if($_POST['mid'] != $data['mid']){ 
                                        ?>
                                            <option value="<?php echo $data['mid']?>"><?php echo htmlspecialchars($data['intitule'])?></option>
                                 <?php }else{ ?>
                                            <option value="<?php echo $data['mid']?>" selected="selected"><?php echo htmlspecialchars($data['intitule'])?></option>
                                    <?php } 
                                    } ?>
                                </select>
                            </td>
                            <td>
                                <select name="gtid" class="form-control">
                                    <?php while($data = $gtid_res->fetch()){
                                        if(strcmp($_POST['gtid'], $data['gtid'])){
                                        ?>
                                            <option value="<?php echo $data['gtid']?>"><?php echo htmlspecialchars($data['nom'])?></option>
                                 <?php }else{ ?>
                                            <option value="<?php echo $data['gtid']?>" selected="selected"><?php echo htmlspecialchars($data['nom'])?></option>
                                    <?php } 
                                    } ?>
                                </select>
                            </td>
                            <td><input type="hidden" name="gid" value="<?php echo $_POST['gid']?>" ></td>
                            <td><button type="button" class="btn" data-toggle="modal" data-target="#<?php echo $donnee['gid']?>"><span class="glyphicon glyphicon-ok" style="color:green;"></span></button></td>
                            <td><button class="btn"><a href="CRUDgroupe.php"><span class="glyphicon glyphicon-ban-circle" style="color:red;"></span></a></button></td>
                            <?php modifModule($donnee['gid'], $donnee['nom']); ?>
                        </tr>
                    </form>
               <?php }else{ ?>
                    <tr>
                        <td><?php echo htmlspecialchars($donnee['nom'])?></td>
                        <td><?php echo htmlspecialchars($donnee['intitule'])?></td>
                        <td><?php echo htmlspecialchars($donnee['gnom'])?></td>
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
            <th>Groupe</th>
            <th>Module</th>
            <th>gtid</th>
        </tr>
    </thead>
    <tbody>
        <?php while($donnee = $stmt->fetch()){ 
            supGroupe($donnee['gid'], $donnee['nom']);
            ?>
        <tr>
            <td><?php echo htmlspecialchars($donnee['nom'])?></td>
            <td><?php echo htmlspecialchars($donnee['intitule'])?></td>
            <td><?php echo htmlspecialchars($donnee['gnom'])?></td>
            <td>
                <form method="post">
                    <input type="hidden" name="mid" value="<?php echo htmlspecialchars($donnee['mid'])?>" >
                    <input type="hidden" name="nom" value="<?php echo htmlspecialchars($donnee['nom'])?>" >
                    <input type="hidden" name="gtid" value="<?php echo htmlspecialchars($donnee['gtid'])?>" >
                    <input type="hidden" name="gid" value="<?php echo htmlspecialchars($donnee['gid'])?>" >
                    <button type="submit" name="modif" class="btn"><span class="glyphicon glyphicon-edit" style="color:rgb(30,144,255);"></span></button>
                </form>
            </td>
            <td><button type="submit" class="btn" data-toggle="modal" data-target="#<?php echo $donnee['gid']?>"><span class="glyphicon glyphicon-remove" style="color:red;"></span></button></td>
        </tr>
        <?php } ?>
            <form method="post">
                <tr>
                    <td>
                        <select name="nom" class="form-control">
                            <?php while($data = $gid_res->fetch()){ ?>
                                <option value="<?php echo $data['nom']?>"><?php echo $data['nom']?></option>
                            <?php } ?>
                        </select>
                    </td>
                    <td>
                        <select name="mid" class="form-control">
                            <?php while($data = $mid_res->fetch()){ ?>
                                <option value="<?php echo $data['mid']?>"><?php echo htmlspecialchars($data['intitule'])?></option>
                           <?php } ?>
                        </select>
                    </td>
                    <td>
                        <select name="gtid" class="form-control">
                            <?php while($data = $gtid_res->fetch()){ ?>
                                <option value="<?php echo $data['gtid']?>"><?php echo htmlspecialchars($data['nom'])?></option>
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