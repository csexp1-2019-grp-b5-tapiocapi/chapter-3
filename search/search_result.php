<html>
<head>
<title>住所検索結果</title>
</head>
<body>
<?php
    $post_data = $_POST["input"];
    echo "<p> [$post_data] の検索結果</p>";
    searchAddr($post_data);

    function searchAddr($post_data) {
        $mysqli = mysqli_connect("localhost", "USER", "PASSWORD", "CSexp1DB");
        if ($mysqli->connect_errno) {
            printf("Connect failed: %s\n", $mysqli->connect_error);
            exit();
        }
        $query = "SELECT zip, addr1, addr2, addr3 FROM zipAll WHERE "
            . "zip      LIKE '"  . $post_data . "%'"
            . "OR kana1 LIKE '%" . $post_data . "%'"
            . "OR kana2 LIKE '%" . $post_data . "%'"
            . "OR kana3 LIKE '%" . $post_data . "%'"
            . "OR addr1 LIKE '%" . $post_data . "%'"
            . "OR addr2 LIKE '%" . $post_data . "%'"
            . "OR addr3 LIKE '%" . $post_data . "%'";

        if ($result = mysqli_query($mysqli, $query)) {
            while($arr = $result->fetch_array(MYSQLI_ASSOC)) {
                foreach($arr as $val) {
                    if (strcmp($val,"以下に掲載がない場合") == 0) {
                        break;
                    }
                    echo "$val ";
                }
                echo "<br>";
            }
        }
        $result->close();
        $mysqli->close();
    }
?>
</body>
</html>
