module.exports = {
    flowFile: "flows.json",
    flowFilePretty: true,
    uiPort: process.env.PORT || 1880,
    uiHost: "0.0.0.0",
    mqttReconnectTime: 15000,
    serialReconnectTime: 15000,
    debugMaxLength: 1000,
    functionExternalModules: false,
    exportGlobalContextKeys: false,
    sqliteReconnectTime: 20000,
    credentialSecret: process.env.NODERED_CREDENTIAL_SECRET || false,
    editorTheme: {
        projects: {
            enabled: false
        }
    },
    dashboard: {
        path: "/dashboard",
        title: "SmartHome Server V1",
        readOnly: false
    }
};
