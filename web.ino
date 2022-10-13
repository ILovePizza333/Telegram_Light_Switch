void newMsg(FB_msg& msg) {
  Serial.print(msg.chatID);  Serial.print(", ");  Serial.print(msg.username);  Serial.print(", ");  Serial.println(msg.text);
  String ledon="/ledon"; ledon+=DEVICE;
  String ledoff="/ledoff"; ledoff+=DEVICE;
  if (msg.text == ledon) {
    sost = 1; prev_sost = sost;
    sendmsg2 = "включен";
    msg.text = sendmsg + sendmsg2;
    bot.sendMessage(msg.text, msg.chatID);
    digitalWrite(out, sost);
  } else if (msg.text == ledoff) {
    sost = 0; prev_sost = sost;
    sendmsg2 = "выключен";
    msg.text = sendmsg + sendmsg2;
    bot.sendMessage(msg.text, msg.chatID);
    digitalWrite(out, sost);
  } else  if (msg.text == "/podval_data") {
    if (sost == 1) {
      sendmsg2 = "включен";
    } else {
      sendmsg2 = "выключен";
    }
    msg.text = sendmsg + sendmsg2;
    bot.sendMessage(msg.text, msg.chatID);
  }

}
