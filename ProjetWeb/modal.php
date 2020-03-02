<?php

function sup($uid, $login){ ?>

<div id="<?php echo htmlspecialchars($uid)?>" class="modal fade">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <h4 class="modal-title text-xs-center">Suppression</h4>
            </div>
            <div class="modal-body">
              <p>Etes vous sur de vouloir supprimer <?php echo htmlspecialchars($login)?> ?<p>
            </div>
            <div class="modal-footer">
                <form role="form" method="POST" action="">
                    <input type="hidden" name="uid" value="<?php echo htmlspecialchars($uid)?>" >
                    <button type="submit" name="sup" class="btn btn-info btn-success">Oui</button>
                    <button type="submit" class="btn btn-info btn-danger" data-dismiss="modal">Non</button>
                </form>
            </div>
        </div>
    </div>
</div>

<?php
}


function modif($uid, $login){ ?>

    <div id="<?php echo htmlspecialchars($uid)?>" class="modal fade">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h4 class="modal-title text-xs-center">Modification</h4>
                </div>
                <div class="modal-body">
                  <p>Etes vous sur de vouloir modifier <?php echo htmlspecialchars($login)?> ?<p>
                </div>
                <div class="modal-footer">
                        <button type="submit" name="modif2" class="btn btn-info btn-success">Oui</button>
                        <button type="submit" class="btn btn-info btn-danger" data-dismiss="modal">Non</button>
                    </form>
                </div>
            </div>
        </div>
    </div>
    
<?php
}

function affect($eid, $nom, $prenom, $login){ ?>

    <div id="<?php echo htmlspecialchars($eid)?>" class="modal fade">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h4 class="modal-title text-xs-center">Affectation</h4>
                </div>
                <div class="modal-body">
                  <p><?php echo "Etes vous sur de vouloir affecter l'utilisateur " . htmlspecialchars($login) . " à l'enseignant " . htmlspecialchars($nom) . " " . htmlspecialchars($prenom) . " ?"?><p>
                </div>
                <div class="modal-footer">
                        <button type="submit" name="affect2" class="btn btn-info btn-success">Oui</button>
                        <button type="submit" class="btn btn-info btn-danger" data-dismiss="modal">Non</button>
                    </form>
                </div>
            </div>
        </div>
    </div>
    
<?php
}

function supEnseignant($eid, $nom, $prenom){ ?>

    <div id="<?php echo htmlspecialchars($eid)?>" class="modal fade">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h4 class="modal-title text-xs-center">Suppression</h4>
                </div>
                <div class="modal-body">
                  <p>Etes vous sur de vouloir supprimer <?php echo htmlspecialchars($nom) . " " . htmlspecialchars($prenom) . " "?> ?<p>
                </div>
                <div class="modal-footer">
                    <form role="form" method="POST" action="">
                        <input type="hidden" name="eid" value="<?php echo htmlspecialchars($eid)?>" >
                        <button type="submit" name="sup" class="btn btn-info btn-success">Oui</button>
                        <button type="submit" class="btn btn-info btn-danger" data-dismiss="modal">Non</button>
                    </form>
                </div>
            </div>
        </div>
    </div>
    
    <?php
    }

    function modifEnseignant($eid, $nom, $prenom){ ?>

        <div id="<?php echo htmlspecialchars($eid)?>" class="modal fade">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h4 class="modal-title text-xs-center">Modification</h4>
                    </div>
                    <div class="modal-body">
                      <p>Etes vous sur de vouloir modifier <?php echo htmlspecialchars($nom) . " " . htmlspecialchars($prenom) . " "?> ?<p>
                    </div>
                    <div class="modal-footer">
                            <button type="submit" name="modif2" class="btn btn-info btn-success">Oui</button>
                            <button type="submit" class="btn btn-info btn-danger" data-dismiss="modal">Non</button>
                        </form>
                    </div>
                </div>
            </div>
        </div>
        
    <?php
    }

    function supModule($mid, $nom){ ?>

        <div id="<?php echo htmlspecialchars($mid)?>" class="modal fade">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h4 class="modal-title text-xs-center">Suppression</h4>
                    </div>
                    <div class="modal-body">
                      <p>Etes vous sur de vouloir supprimer <?php echo htmlspecialchars($nom)?> ?<p>
                    </div>
                    <div class="modal-footer">
                        <form role="form" method="POST" action="">
                            <input type="hidden" name="mid" value="<?php echo htmlspecialchars($mid)?>" >
                            <button type="submit" name="sup" class="btn btn-info btn-success">Oui</button>
                            <button type="submit" class="btn btn-info btn-danger" data-dismiss="modal">Non</button>
                        </form>
                    </div>
                </div>
            </div>
        </div>
        
        <?php
        }

        function modifModule($mid, $nom){ ?>

            <div id="<?php echo htmlspecialchars($mid)?>" class="modal fade">
                <div class="modal-dialog">
                    <div class="modal-content">
                        <div class="modal-header">
                            <h4 class="modal-title text-xs-center">Modification</h4>
                        </div>
                        <div class="modal-body">
                          <p>Etes vous sur de vouloir modifier <?php echo htmlspecialchars($nom)?> ?<p>
                        </div>
                        <div class="modal-footer">
                                <button type="submit" name="modif2" class="btn btn-info btn-success">Oui</button>
                                <button type="submit" class="btn btn-info btn-danger" data-dismiss="modal">Non</button>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
            
        <?php
        } 

        function modifGroupe($gid, $nom){ ?>

            <div id="<?php echo htmlspecialchars($gid)?>" class="modal fade">
                <div class="modal-dialog">
                    <div class="modal-content">
                        <div class="modal-header">
                            <h4 class="modal-title text-xs-center">Modification</h4>
                        </div>
                        <div class="modal-body">
                          <p>Etes vous sur de vouloir modifier <?php echo htmlspecialchars($nom)?> ?<p>
                        </div>
                        <div class="modal-footer">
                                <button type="submit" name="modif2" class="btn btn-info btn-success">Oui</button>
                                <button type="submit" class="btn btn-info btn-danger" data-dismiss="modal">Non</button>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
            
        <?php
        }   

        function supGroupe($gid, $nom){ ?>

            <div id="<?php echo htmlspecialchars($gid)?>" class="modal fade">
                <div class="modal-dialog">
                    <div class="modal-content">
                        <div class="modal-header">
                            <h4 class="modal-title text-xs-center">Suppression</h4>
                        </div>
                        <div class="modal-body">
                          <p>Etes vous sur de vouloir supprimer <?php echo htmlspecialchars($nom)?> ?<p>
                        </div>
                        <div class="modal-footer">
                            <form role="form" method="POST" action="">
                                <input type="hidden" name="gid" value="<?php echo htmlspecialchars($gid)?>" >
                                <button type="submit" name="sup" class="btn btn-info btn-success">Oui</button>
                                <button type="submit" class="btn btn-info btn-danger" data-dismiss="modal">Non</button>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
            
            <?php
            }
?>              