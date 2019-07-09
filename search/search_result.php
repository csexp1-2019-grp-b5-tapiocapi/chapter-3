<!DOCTYPE HTML>
<html lang=ja>
<head>
<!-- Required meta tags -->
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

<!-- Bootstrap CSS -->
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<title>住所検索結果</title>
</head>
<body>
<?php
    $mysqli = mysqli_connect("localhost", "cs17075", "Nsmhrmlo2", "CSexp1DB");
    $post_data = $_POST["input"];
    if (mysqli_connect_errno()){
        printf("Connect failed: %s\n", $mysqli->connect_error);
        exit();
    }
    echo "<p> [$post_data] の検索結果</p>";

    $result = searchAddr($mysqli, $post_data);
    if ($result->num_rows == 0) {
        echo "<p> 該当する住所がありませんでした</p>";
    } else {
        echo "<p>[" . $result->num_rows . "]件のヒット</p>";
        printResult($result);
    }

    mysqli_close($result);
    mysqli_close($mysqli);

    function searchAddr($mysqli ,$post_data) {
        $query = "SELECT zip, addr1, addr2, addr3 FROM zipAll WHERE "
            . "zip      LIKE ? "
            . "OR kana1 LIKE ? "
            . "OR kana2 LIKE ? "
            . "OR kana3 LIKE ? "
            . "OR addr1 LIKE ? "
            . "OR addr2 LIKE ? "
            . "OR addr3 LIKE ? ";
        $stmt = mysqli_prepare($mysqli, $query);
        if (!$stmt) {
            echo mysqli_error($mysqli);
            exit();
        }
        $post_data = "%$post_data%";
        mysqli_stmt_bind_param($stmt, "sssssss",$post_data, $post_data, $post_data, $post_data, $post_data, $post_data, $post_data);

        mysqli_stmt_execute($stmt);

        return mysqli_stmt_get_result($stmt);
    }

    function printResult($result) {
        echo '<table class="table table-bordered">'
            .'<thread>'
            .'<th>郵便番号</th>'
            .'<th>住所</th>'
            .'</thread>'
            .'<tbody>';
        while($arr = mysqli_fetch_array($result, MYSQLI_ASSOC)) {
            $zip = $arr["zip"];
            if (strcmp($arr["addr3"], "以下に掲載がない場合") == 0) {
                $address = $arr["addr1"] . $arr["addr2"];
            } else {
                $address = $arr["addr1"] . $arr["addr2"] . $arr["addr3"];
            }
            echo "<tr>"
                    ."<td>" . $zip     ."</td>"
                    ."<td>" . $address ."</td>"
                ."</tr>";
        }
        echo '</tbody>'
            .'</table>';
    }
?>
</body>
</html>
