<!DOCTYPE html>
<html lang="ja">
    <head>
        <title>実装Aの結果</title>
    </head>
    <body>
<?php
//エラーを出力する
ini_set('display_errors', "On");
?>
<?php
if(isset($_REQUEST["tag"])){
    // echo "input:".$_REQUEST["tag"];
    exec("./client ".$_REQUEST["tag"], $result);
    //print_r ($result);
?>
    <table border="1" width="1000" cellspacing="0" cellpadding="5">
        <thead>
        <th bgcolor=#EE0000 scope="col">image</th>
        <th bgcolor=#EE0000 scope="col">time</th>
        <th bgcolor=#EE0000 scope="col">latitude</th>
        <th bgcolor=#EE0000 scope="col">longitude</th>
        <th bgcolor=#EE0000 scope="col">url</th>
        <th bgcolor=#EE0000 scope="col">map</th>
        </thead>
        <tbody>
<?php
    for ($i=0; $i<count($result); $i++){
?>
        <tr>
        <td><img src=<?php echo $result[$i*4+3] ?> width=150></td>
        <td><?php echo $result[$i*4+0] ?></td>
        <td><?php echo $result[$i*4+1] ?></td>
        <td><?php echo $result[$i*4+2] ?></td>
        <td><?php echo $result[$i*4+3] ?></td>
        </tr>
<?php
    }
}
?>
        </tbody>
    </table>
    </body>
</html>
