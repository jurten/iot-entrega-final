# node-red-contrib-mqtt-auto 📜

This node is intended to facilitate connection and change of connection credentials at runtime.

## Nodes ☕️

Diga adeus aos perfis sem graça. Com nossos **[templates de perfil](https://github.com/iuricode/readme-template/tree/main/perfil)**, você terá um readme de perfil íncrivel. Com cores vibrantes,imagens surpreendentes e outros elementos visuais cativantes.

### mqtt-auto-connect
<img src="img/connects.png" width="200" alt="Conectar">



```
msg.payload = {

  "client_id": "clientid",
  "host": "locahost",
  "user": "user",
  "password": "password",
  "reconnect_t" : 1, // 0 No Reconect  1 AutoReconnect
};
```


### mqtt-auto-disconnect

<img src="img/disconnect.png" width="200" alt="Conectar">


```
msg.client_id = "client_id";
```

### mqtt-auto-status

<img src="img/status.png" width="200" alt="Conectar">



### mqtt-auto-subscribe

<img src="img/subscribe.png" width="200" alt="Conectar">


```
msg.client_id = "client_id";
msg.topic = "topic";
```


### mqtt-auto-unsubscribe

<img src="img/unsubscribe.png" width="200" alt="Conectar">

```
msg.client_id = "client_id";
msg.topic = "topic";
```




## Thanks 🔥



This project was based on the project of **[promd76](https://www.npmjs.com/~promd76)**

## Donate ☕️

If this project was useful to you, help me continue developing and drinking a coffee while I'm coding.

<p>
  <a href="https://www.paypal.com/donate/?business=A7LJZ2W82R8NQ&no_recurring=0&item_name=Me+ajuda+com+meu+cafezinho.%0Acollaborate+with+my+coffee.&currency_code=BRL">
      <img src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif" alt="paypal">

  </a>
</p>



| ![GRCode PIX](img/qrcode2.jpeg) | ![GRCode PayPal](img/qrcode.png) |
|-------------------------------|----------------------------------|
|       GRCode PIX              | GRCode PayPal                    |
