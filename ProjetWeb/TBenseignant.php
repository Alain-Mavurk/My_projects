<?php

require("auth/EtreAuthentifie.php");

$title = 'TBenseignant';
include("header.php");

if(!strcmp($idm->getRole(), "user")){

    $SQL = "SELECT intitule, g.nom, SUM(a.nbh) nbh, etypes.nbh nbht, SUM(coeff*a.nbh) eqtd FROM enseignants e
        JOIN users u on u.uid = e.uid
        JOIN affectations a on a.eid = e.eid
        JOIN groupes g on g.gid = a.gid
        JOIN modules m on m.mid = g.mid
        JOIN gtypes on gtypes.gtid = g.gtid
        JOIN etypes on etypes.etid = e.etid
        WHERE u.uid = :uid AND e.annee = :annee
        GROUP BY intitule, g.nom, etypes.etid
    ";
    $stmt = $db->prepare($SQL);
    $stmt->execute(array(
        'uid' => $idm->getUid(),
        'annee' => $_SESSION['annee']
    ));

    $nbh = 0;
    $eqtd = 0;
    $nbhr = -1;
    ?>
        <table class="table table-hover">
            <thead class="black white-text">
                <tr>
                    <th>Modules</th>
                    <th>Groupes</th>
                    <th>Nbh</th>
                    <th>Eqtd</th>
                    <th>Nbh restant</th>
                </tr>
            </thead>
            <tbody>
                <?php while($res = $stmt->fetch()){
                    if($nbhr == -1) $nbhr = $res['nbht'];
                    $nbh += $res['nbh'];
                    $eqtd += (int)$res['eqtd'];
                    $nbhr -= $res['nbh'];
                    if($nbhr < 0) $nbhr = 0;
                    ?>
                    
                <tr>
                    <td><?php echo htmlspecialchars($res['intitule']);?></td>
                    <td><?php echo htmlspecialchars($res['nom']);?></td>
                    <td><?php echo htmlspecialchars($res['nbh']);?></td>
                    <td><?php echo htmlspecialchars((int)$res['eqtd']);?></td>
                    <td></td>
                </tr>
                <?php }?>
                <tr>
                    <td>Total</td>
                    <td></td>
                    <td><?php echo htmlspecialchars($nbh)?></td>
                    <td><?php echo htmlspecialchars($eqtd);?></td>
                    <td><?php echo htmlspecialchars($nbhr);?></td>
                </tr>
            </tbody>
        </table>

<?php
    $stmt->closeCursor();

}else{
    $SQL = "SELECT eid, nom, prenom FROM enseignants e
        WHERE e.annee = :annee
    ";
    $stmt = $db->prepare($SQL);
    $res = $stmt->execute(array(
        'annee' => $_SESSION['annee']
    ));

    $i = 0;
        ?>
        <form action="<?= $pathFor['TBenseignant'] ?>" method="post">
            <select name="enseignant" style="width: auto;" class="form-control" onchange="this.form.submit();">
                <?php while($res = $stmt->fetch()){ 
                    $eid = htmlspecialchars($res['eid']);
                    if(!isset($firstEid)) $firstEid = htmlspecialchars($res['eid']);
                    $nom = htmlspecialchars($res['nom']);
                    $prenom = htmlspecialchars($res['prenom']);
                    if(isset($_POST['enseignant'])){
                        if(strcmp($eid, $_POST['enseignant'])){ ?>
                            <option value="<?php echo $eid;?>" ><?php echo $nom . ' ' . $prenom;?></option>
                        <?php }else{?>
                            <option value="<?php echo $eid;?>" selected="selected"><?php echo $nom . ' ' . $prenom;?></option>
                        <?php }
                    }else{?>
                        <option value="<?php echo $eid;?>" ><?php echo $nom . ' ' . $prenom;?></option>
                    <?php }
                    $i++;
                }
                ?>
            </select>
        </form>
        <br>
        <?php 

        if($i == 0) $firstEid = 0;

        if(isset($_POST['enseignant']))
            $eid = $_POST['enseignant'];
        else
            $eid = $firstEid;    
            $SQL = "SELECT intitule, g.nom, SUM(a.nbh) nbh, etypes.nbh nbht, SUM(coeff*a.nbh) eqtd FROM enseignants e
            JOIN affectations a on a.eid = e.eid
            JOIN groupes g on g.gid = a.gid
            JOIN modules m on m.mid = g.mid
            JOIN gtypes on gtypes.gtid = g.gtid
            JOIN etypes on etypes.etid = e.etid
            WHERE e.eid = :eid
            GROUP BY intitule, g.nom
        ";
        $stmt = $db->prepare($SQL);
        $stmt->execute(array(
            'eid' => $eid
        ));

        $nbh = 0;
        $eqtd = 0;
        $nbhr = 0;
        ?>
            <table class="table table-hover">
                <thead class="black white-text">
                    <tr>
                        <th>Modules</th>
                        <th>Groupes</th>
                        <th>Nbh</th>
                        <th>Eqtd</th>
                        <th>Nbh restant</th>
                    </tr>
                </thead>
                <tbody>
                    <?php while($res = $stmt->fetch()){
                        if($nbhr == 0) $nbhr = $res['nbht'];
                        $nbh += $res['nbh'];
                        $eqtd += (int)$res['eqtd'];
                        $nbhr -= $res['nbh'];
                        if($nbhr < 0) $nbhr = 0;
                        ?>
                        
                    <tr>
                        <td><?php echo htmlspecialchars($res['intitule']);?></td>
                        <td><?php echo htmlspecialchars($res['nom']);?></td>
                        <td><?php echo htmlspecialchars($res['nbh']);?></td>
                        <td><?php echo htmlspecialchars((int)$res['eqtd']);?></td>
                        <td></td>
                    </tr>
                    <?php }?>
                    <tr>
                        <td>Total</td>
                        <td></td>
                        <td><?php echo htmlspecialchars($nbh);?></td>
                        <td><?php echo htmlspecialchars($eqtd);?></td>
                        <td><?php echo htmlspecialchars($nbhr);?></td>
                    </tr>
                </tbody>
            </table>

    <?php
        $stmt->closeCursor();
}
?>




