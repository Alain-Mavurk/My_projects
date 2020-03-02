<?php

require("auth/EtreAuthentifie.php");

$title = 'TBmodules';
include("header.php");

if(!strcmp($idm->getRole(), "admin")){
    $SQL = "SELECT intitule FROM modules m
        GROUP BY intitule
    ";
    $stmt = $db->prepare($SQL);
    $stmt->execute();
        ?>
        <form action="<?= $pathFor['TBmodules'] ?>" method="post">
            <select name="module" style="width: auto;" class="form-control" onchange="this.form.submit();">
                <?php while($res = $stmt->fetch()){
                    $nom = htmlspecialchars($res['intitule']);
                    if(!isset($firstNom)) $firstNom = htmlspecialchars($res['intitule']);
                    if(isset($_POST['module'])){
                        if(strcmp($res['intitule'], $_POST['module'])){ ?>
                            <option value="<?php echo $nom;?>" ><?php echo $nom;?></option>
                        <?php }else{?>
                            <option value="<?php echo $nom;?>" selected="selected"><?php echo $nom;?></option>
                        <?php }
                    }else{?>
                        <option value="<?php echo $nom;?>" ><?php echo $nom;?></option>
                    <?php }
                }
                ?>
            </select>
        </form>
<br>
        <?php 
        if(isset($_POST['module']))
            $intitule = $_POST['module'];
        else
            $intitule = $firstNom;       
        $SQL = "SELECT e.nom, e.prenom, SUM(a.nbh) nbh, gtypes.nbh nbht, SUM(coeff*a.nbh) eqtd FROM affectations a
            JOIN enseignants e on e.eid = a.eid
            JOIN groupes g on a.gid = g.gid
            RIGHT OUTER JOIN modules m on g.mid = m.mid
            JOIN gtypes on gtypes.gtid = g.gtid
            WHERE intitule = :intitule AND m.annee = :annee
            GROUP BY intitule, e.nom, e.prenom, gtypes.nbh
        ";
        $stmt = $db->prepare($SQL);
        $stmt->execute(array(
            'intitule' => $intitule,
            'annee' => $_SESSION['annee']
        ));

        $nbh = 0;
        $eqtd = 0;
        $nbhr = 0;
        ?>
            <table class="table table-hover">
                <thead class="black white-text">
                    <tr>
                        <th>Enseignants</th>
                        <th>Nbh</th>
                        <th>Eqtd</th>
                        <th>Nbh restant</th>
                    </tr>
                </thead>
                <tbody>
                    <?php while($res = $stmt->fetch()){
                        $nbh += $res['nbh'];
                        $eqtd += (int)$res['eqtd'];
                        $nbhr = $res['nbht'] - $res['nbh'];
                        ?>
                        
                    <tr>
                        <td><?php echo htmlspecialchars($res['nom']) . ' ' . htmlspecialchars($res['prenom']);?></td>
                        <td><?php echo htmlspecialchars($res['nbh']);?></td>
                        <td><?php if((int)$res['eqtd'] != 0) echo htmlspecialchars((int)$res['eqtd']);?></td>
                        <td><?php if($res['nbh']) echo htmlspecialchars($nbhr);?></td>
                    </tr>
                    <?php }?>
                    <tr>
                        <td>Total</td>
                        <td></td>
                        <td><?php echo htmlspecialchars($nbh);?></td>
                        <td><?php echo htmlspecialchars($eqtd);?></td>
                    </tr>
                </tbody>
            </table>

    <?php
        $stmt->closeCursor();
}
?>