<?php

require("auth/EtreAuthentifie.php");

$title = 'Accueil';
include("header.php");


echo "Hello " . $idm->getIdentity().". Your uid is: ". $idm->getUid() .". Your role is: ".$idm->getRole();

$SQL ="SELECT MIN(annee) min, MAX(annee) max FROM enseignants
    LIMIT 1
";

$stmt = $db->prepare($SQL);
$stmt->execute();
$res = $stmt->fetch();

if($res){
    $annee = htmlspecialchars($res['min']);
    $max = htmlspecialchars($res['max']);
}else{
    $annee = $_SESSION['annee'];
}
?>
<br>
<br>
 

<form action="" method="post">
    <select name="annee" style="width:auto;" class="form-control" onchange="this.form.submit();">
        <?php if($res){
            while($annee <= $max){
                if(isset($_POST['annee'])){
                    if(strcmp($_POST['annee'], $annee)){?>
                        <option value="<?php echo $annee;?>" ><?php echo $annee;?></option>
                    <?php }else{?>
                        <option value="<?php echo $_SESSION['annee'];?>" selected="selected"><?php echo $_SESSION['annee'];?></option>
                    <?php } 
                }else{
                    if(strcmp($_SESSION['annee'], $annee)){?>
                        <option value="<?php echo $annee;?>" ><?php echo $annee;?></option>
                    <?php }else{?>
                        <option value="<?php echo $_SESSION['annee'];?>" selected="selected"><?php echo $_SESSION['annee'];?></option>
                    <?php }
                }
                $annee++;
            }
        }else{?>
            <option value="<?php echo $annee;?>" selected="selected"><?php echo $annee;?></option>
        <?php }
        ?>
    </select>
</form>   
<br>



<?

//echo "Escaped values: ".$e_($ci->idm->getIdentity());


include("footer.php");

?>