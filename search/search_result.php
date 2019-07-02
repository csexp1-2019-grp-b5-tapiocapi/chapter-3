<?php
    $post_data = $_POST["input"];
    $mysqli = mysqli_connect("localhost", "DBroot", "shizudai", "CSexp1DB");
    if ($mysqli->connect_errno) {
        printf("Connect failed: %s\n", $mysqli->connect_error);
        exit();
    }
    $query = "SELECT * FROM zipShizuoka WHERE zip LIKE '" . $post_data . "%'"
        . "OR kana1 LIKE '" . $post_data . "%'"
        . "OR kana2 LIKE '" . $post_data . "%'"
        . "OR kana3 LIKE '" . $post_data . "%'"
        . "OR addr1 LIKE '" . $post_data . "%'"
        . "OR addr2 LIKE '" . $post_data . "%'"
        . "OR addr3 LIKE '" . $post_data . "%'";
    sprintf($query, $input);

    if ($result = mysqli_query($mysqli, $query)) {
        while($arr = $result->fetch_array(MYSQLI_NUM)) {
            foreach($arr as $val) {
                echo "$val ";
            }
            echo "<br>";
        }

    }

    $result->close();
    $mysqli->close();
?>

