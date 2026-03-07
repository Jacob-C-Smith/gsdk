go build g_web.go
sudo systemctl disable sdk.g10.app.service
cp g_web /usr/local/bin/sdk_g10_app
sudo systemctl daemon-reload
sudo systemctl enable sdk.g10.app.service
sudo systemctl start sdk.g10.app.service
sudo systemctl status sdk.g10.app.service