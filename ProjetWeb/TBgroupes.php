<?php

require("auth/EtreAuthentifie.php");

$title = 'TBgroupes';
include("header.php");

if(!strcmp($idm->getRole(), "admin")){
    $SQL = "SELECT nom FROM groupes g
        GROUP BY nom
    ";
    $stmt = $db->prepare($SQL);
    $stmt->execute();
        ?>
        <form action="<?= $pathFor['TBgroupes'] ?>" method="post">
            <select name="groupe" class="form-control" style="width: 10%;" onchange="this.form.submit();">
                <?php while($res = $stmt->fetch()){
                    $nom = htmlspecialchars($res['nom']);
                    if(!isset($firstNom)) $firstNom = htmlspecialchars($res['nom']);
                    if(isset($_POST['groupe'])){
                        if(strcmp($res['nom'], $_POST['groupe'])){ ?>
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
        if(isset($_POST['groupe']))
            $gnom = $_POST['groupe'];
        else
            $gnom = $firstNom;
        $SQL = "SELECT intitule, e.nom, e.prenom, SUM(a.nbh) nbh, gtypes.nbh nbht, SUM(coeff*a.nbh) eqtd FROM affectations a
            JOIN enseignants e on e.eid = a.eid
            RIGHT OUTER JOIN groupes g on a.gid = g.gid
            JOIN modules m on m.mid = g.mid
            JOIN gtypes on gtypes.gtid = g.gtid
            WHERE g.nom = :gnom AND g.annee = :annee
            GROUP BY intitule, e.nom, e.prenom, gtypes.nbh
        ";
        $stmt = $db->prepare($SQL);
        $stmt->execute(array(
            'gnom' => $gnom,
            'annee' => $_SESSION['annee']
        ));

        $nbh = 0;
        $eqtd = 0;
        $nbhr = 0;
        ?>
            <table class="table table-hover">
                <thead class="black white-text">
                    <tr>
                        <th>Modules</th>
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
                        <td><?php echo htmlspecialchars($res['intitule']);?></td>
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
                        <td></td>
                    </tr>
                </tbody>
            </table>

    <?php
        $stmt->closeCursor();
}
?>




