-- 테스트용 DB (logintest)를 만드는 쿼리문 

CREATE TABLE IF NOT EXISTS `logintest`.`member` (
  `email` VARCHAR(50) NOT NULL,
  `password` VARCHAR(255) NOT NULL,
  `qrcode` VARCHAR(255) NOT NULL,
  PRIMARY KEY (`email`),
  UNIQUE INDEX `email_UNIQUE` (`email` ASC) VISIBLE,
  UNIQUE INDEX `QRCode_UNIQUE` (`qrcode` ASC) VISIBLE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci