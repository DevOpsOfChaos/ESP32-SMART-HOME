const nodes = [];

const addNode = (node) => nodes.push(node);
const script = (...lines) => lines.join("\n");

const ids = {
    tabIngest: "tab_ingest",
    tabRegistry: "tab_registry",
    tabState: "tab_state",
    tabConfig: "tab_config",
    tabTimeSeries: "tab_time_series",
    tabAutomation: "tab_automation",
    tabCommand: "tab_command",
    tabDashboardUi: "tab_dashboard_ui",
    tabDashboardSim: "tab_dashboard_sim",
    tabLogging: "tab_logging",
    mqttBroker: "cfg_mqtt_broker",
    sqliteDb: "cfg_sqlite_db",
    uiBase: "cfg_ui_base",
    uiThemeDefault: "cfg_ui_theme_default",
    uiPageOverview: "cfg_ui_page_overview",
    uiPageRooms: "cfg_ui_page_rooms",
    uiPageDevices: "cfg_ui_page_devices",
    uiPageDeviceDetail: "cfg_ui_page_device_detail",
    uiPageSensors: "cfg_ui_page_sensors",
    uiPageActors: "cfg_ui_page_actors",
    uiPageCovers: "cfg_ui_page_covers",
    uiPageWeather: "cfg_ui_page_weather",
    uiPageCharts: "cfg_ui_page_charts",
    uiPageAutomations: "cfg_ui_page_automations",
    uiPageConfig: "cfg_ui_page_config",
    uiPageLogs: "cfg_ui_page_logs",
    uiPageSimulation: "cfg_ui_page_simulation",
    uiGroupOverview: "cfg_ui_group_overview",
    uiGroupRooms: "cfg_ui_group_rooms",
    uiGroupDevices: "cfg_ui_group_devices",
    uiGroupDeviceDetail: "cfg_ui_group_device_detail",
    uiGroupSensors: "cfg_ui_group_sensors",
    uiGroupActors: "cfg_ui_group_actors",
    uiGroupCovers: "cfg_ui_group_covers",
    uiGroupWeather: "cfg_ui_group_weather",
    uiGroupCharts: "cfg_ui_group_charts",
    uiGroupAutomations: "cfg_ui_group_automations",
    uiGroupConfig: "cfg_ui_group_config",
    uiGroupLogs: "cfg_ui_group_logs",
    uiGroupSimControls: "cfg_ui_group_sim_controls",
    uiGroupSimOverview: "cfg_ui_group_sim_overview",
    uiTemplateOverview: "ui_template_overview",
    uiTemplateRooms: "ui_template_rooms",
    uiTemplateDevices: "ui_template_devices",
    uiTemplateDeviceDetail: "ui_template_device_detail",
    uiTemplateSensors: "ui_template_sensors",
    uiTemplateActors: "ui_template_actors",
    uiTemplateCovers: "ui_template_covers",
    uiTemplateWeather: "ui_template_weather",
    uiTemplateCharts: "ui_template_charts",
    uiTemplateAutomations: "ui_template_automations",
    uiTemplateConfig: "ui_template_config",
    uiTemplateLogs: "ui_template_logs",
    uiTemplateHeader: "ui_template_header",
    mqttServerStatus: "mqtt_server_status",
    mqttMasterStatus: "mqtt_master_status",
    mqttMasterEvent: "mqtt_master_event",
    mqttNodeMeta: "mqtt_node_meta",
    mqttNodeStatus: "mqtt_node_status",
    mqttNodeState: "mqtt_node_state",
    mqttNodeEvent: "mqtt_node_event",
    mqttNodeAck: "mqtt_node_ack",
    mqttNodeCfgReport: "mqtt_node_cfg_report",
    normalize: "fn_normalize",
    routeCore: "fn_route_core",
    linkOutDeviceRegistry: "link_out_device_registry",
    linkInDeviceRegistry: "link_in_device_registry",
    buildDeviceUpsert: "fn_build_device_upsert",
    sqliteDeviceUpsert: "sqlite_device_upsert",
    routePostRegistry: "fn_route_post_registry",
    buildCapabilityUpserts: "fn_build_capability_upserts",
    sqliteCapabilityUpserts: "sqlite_capability_upserts",
    linkOutStateStore: "link_out_state_store",
    linkInStateStore: "link_in_state_store",
    buildLastStateUpsert: "fn_build_last_state_upsert",
    sqliteLastStateUpsert: "sqlite_last_state_upsert",
    linkOutServerStatus: "link_out_server_status",
    linkInServerStatus: "link_in_server_status",
    buildServerStatusUpserts: "fn_build_server_status_upserts",
    sqliteServerStatusUpserts: "sqlite_server_status_upserts",
    linkOutConfigStore: "link_out_config_store",
    linkInConfigStore: "link_in_config_store",
    buildConfigUpserts: "fn_build_config_upserts",
    sqliteConfigUpserts: "sqlite_config_upserts",
    linkOutTimeSeries: "link_out_time_series",
    linkInTimeSeries: "link_in_time_series",
    linkOutAutomationState: "link_out_automation_state",
    linkInAutomationState: "link_in_automation_state",
    buildInfluxLine: "fn_build_influx_line",
    influxWrite: "http_influx_write",
    uiButtonRefreshSim: "ui_button_refresh_sim",
    uiTemplateSimOverview: "ui_template_sim_overview",
    injectNetErlOn: "inject_net_erl_on",
    injectNetErlOff: "inject_net_erl_off",
    injectNetZrlRelay1On: "inject_net_zrl_relay_1_on",
    injectNetZrlRelay1Off: "inject_net_zrl_relay_1_off",
    injectNetZrlRelay2On: "inject_net_zrl_relay_2_on",
    injectNetZrlRelay2Off: "inject_net_zrl_relay_2_off",
    injectSimDashboardLoad: "inject_sim_dashboard_load",
    injectAutomationTimeTick: "inject_automation_time_tick",
    buildNetErlCmdSet: "fn_build_net_erl_cmd_set",
    buildNetZrlCmdSet: "fn_build_net_zrl_cmd_set",
    buildGenericCommand: "fn_build_generic_command",
    routeDeviceDatasetAction: "fn_route_device_dataset_action",
    buildDeviceDatasetQuery: "fn_build_device_dataset_query",
    sqliteDeviceDatasetQuery: "sqlite_device_dataset_query",
    buildDeviceDatasetPayload: "fn_build_device_dataset_payload",
    routeDeviceDatasetPayload: "fn_route_device_dataset_payload",
    routeDetailAction: "fn_route_detail_action",
    buildDeviceDetailQuery: "fn_build_device_detail_query",
    sqliteDeviceDetailQuery: "sqlite_device_detail_query",
    buildDeviceDetailPayload: "fn_build_device_detail_payload",
    buildDeviceSaveStatements: "fn_build_device_save_statements",
    buildDeviceDeleteStatements: "fn_build_device_delete_statements",
    sqliteDeviceDetailWrite: "sqlite_device_detail_write",
    routePostDeviceDetailWrite: "fn_route_post_device_detail_write",
    routeWeatherAction: "fn_route_weather_action",
    buildWeatherQuery: "fn_build_weather_query",
    sqliteWeatherQuery: "sqlite_weather_query",
    buildWeatherPayload: "fn_build_weather_payload",
    buildWeatherSaveStatements: "fn_build_weather_save_statements",
    sqliteWeatherWrite: "sqlite_weather_write",
    routePostWeatherWrite: "fn_route_post_weather_write",
    routeHeaderAction: "fn_route_header_action",
    buildHeaderQuery: "fn_build_header_query",
    sqliteHeaderQuery: "sqlite_header_query",
    buildHeaderPayload: "fn_build_header_payload",
    routeAutomationAction: "fn_route_automation_action",
    buildAutomationQuery: "fn_build_automation_query",
    sqliteAutomationQuery: "sqlite_automation_query",
    buildAutomationPayload: "fn_build_automation_payload",
    buildAutomationWriteStatements: "fn_build_automation_write_statements",
    sqliteAutomationWrite: "sqlite_automation_write",
    routePostAutomationWrite: "fn_route_post_automation_write",
    routeLogsAction: "fn_route_logs_action",
    buildLogsQuery: "fn_build_logs_query",
    sqliteLogsQuery: "sqlite_logs_query",
    buildLogsPayload: "fn_build_logs_payload",
    routeConfigAction: "fn_route_config_action",
    buildConfigPageQuery: "fn_build_config_page_query",
    sqliteConfigPageQuery: "sqlite_config_page_query",
    buildConfigPagePayload: "fn_build_config_page_payload",
    buildChartRequest: "fn_build_chart_request",
    influxChartQuery: "http_influx_chart_query",
    parseChartResponse: "fn_parse_chart_response",
    routeChartPayload: "fn_route_chart_payload",
    routeSimDashboardRefresh: "fn_route_sim_dashboard_refresh",
    buildSimDashboardQuery: "fn_build_sim_dashboard_query",
    sqliteSimDashboardQuery: "sqlite_sim_dashboard_query",
    buildSimDashboardMarkdown: "fn_build_sim_dashboard_markdown",
    uiEventDashboard: "ui_event_dashboard",
    buildSensorAutomationQuery: "fn_build_sensor_automation_query",
    sqliteSensorAutomationQuery: "sqlite_sensor_automation_query",
    evaluateSensorAutomations: "fn_evaluate_sensor_automations",
    buildTimeAutomationQuery: "fn_build_time_automation_query",
    sqliteTimeAutomationQuery: "sqlite_time_automation_query",
    evaluateTimeAutomations: "fn_evaluate_time_automations",
    buildAutomationExecutionUpdate: "fn_build_automation_execution_update",
    sqliteAutomationExecutionUpdate: "sqlite_automation_execution_update",
    mqttCmdSetOut: "mqtt_cmd_set_out",
    linkOutAuditEgress: "link_out_audit_egress",
    linkOutAudit: "link_out_audit",
    linkInAudit: "link_in_audit",
    buildAuditInsert: "fn_build_audit_insert",
    sqliteAuditInsert: "sqlite_audit_insert"
};

const toSqlLines = [
    'const toSql = (value) => {',
    '    if (value === null || value === undefined) {',
    '        return "NULL";',
    '    }',
    '    if (typeof value === "number") {',
    '        return Number.isFinite(value) ? String(value) : "NULL";',
    '    }',
    '    if (typeof value === "boolean") {',
    '        return value ? "1" : "0";',
    '    }',
    '    return "\'" + String(value).replace(/\'/g, "\'\'") + "\'";',
    '};'
];

const pickStringLines = [
    'const pickString = (...values) => {',
    '    for (const value of values) {',
    '        if (typeof value === "string" && value.trim()) {',
    '            return value.trim();',
    '        }',
    '    }',
    '    return null;',
    '};'
];

const routeCoreFunc = script(
    'const event = msg.payload || {};',
    'if (event.scope === "server" && event.channel === "status") {',
    '    return [null, msg];',
    '}',
    'if (event.scope === "master" || event.scope === "node") {',
    '    return [msg, null];',
    '}',
    'return null;'
);

const normalizeFunc = script(
    'const topic = String(msg.topic || "");',
    'const parts = topic.split("/");',
    'if (parts[0] !== "smarthome") {',
    '    return null;',
    '}',
    'let scope = null;',
    'let entityId = null;',
    'let channel = null;',
    'if (parts.length === 3 && parts[1] === "server" && parts[2] === "status") {',
    '    scope = "server";',
    '    channel = "status";',
    '} else if (parts.length === 4 && parts[1] === "master" && ["status", "event"].includes(parts[3])) {',
    '    scope = "master";',
    '    entityId = parts[2];',
    '    channel = parts[3];',
    '} else if (parts.length === 4 && parts[1] === "node" && ["meta", "status", "state", "event", "ack"].includes(parts[3])) {',
    '    scope = "node";',
    '    entityId = parts[2];',
    '    channel = parts[3];',
    '} else if (parts.length === 5 && parts[1] === "node" && parts[3] === "cfg" && parts[4] === "report") {',
    '    scope = "node";',
    '    entityId = parts[2];',
    '    channel = "cfg/report";',
    '} else {',
    '    return null;',
    '}',
    'let parsed = msg.payload;',
    'if (Buffer.isBuffer(parsed)) {',
    '    parsed = parsed.toString("utf8");',
    '}',
    'if (typeof parsed === "string") {',
    '    try {',
    '        parsed = JSON.parse(parsed);',
    '    } catch (error) {',
    '        node.warn("Dropping invalid JSON on " + topic + ": " + error.message);',
    '        return null;',
    '    }',
    '}',
    'if (!parsed || typeof parsed !== "object" || Array.isArray(parsed)) {',
    '    node.warn("Dropping non-object payload on " + topic);',
    '    return null;',
    '}',
    'const receivedAt = new Date().toISOString();',
    'const payloadIdCandidates = scope === "master"',
    '    ? [parsed.master_id, parsed.device_id, parsed.id]',
    '    : scope === "node"',
    '        ? [parsed.device_id, parsed.node_id, parsed.id]',
    '        : [parsed.server_id, parsed.id];',
    'const payloadId = payloadIdCandidates.find((value) => typeof value === "string" && value.trim()) || null;',
    'if (entityId && payloadId && payloadId !== entityId) {',
    '    node.warn("Dropping payload with mismatched id on " + topic);',
    '    return null;',
    '}',
    'if ((channel === "event" || channel === "ack") && msg.retain) {',
    '    node.warn("Unexpected retained " + channel + " on " + topic + "; message stays out of current-state storage.");',
    '    }',
    'const ts = [parsed.ts, parsed.timestamp, parsed.occurred_at, parsed.reported_at]',
    '    .find((value) => typeof value === "string" && value.trim()) || receivedAt;',
    'const masterId = scope === "master"',
    '    ? entityId',
    '    : (typeof parsed.master_id === "string" && parsed.master_id.trim() ? parsed.master_id.trim() : null);',
    'msg.payload = {',
    '    topic,',
    '    scope,',
    '    entityId,',
    '    channel,',
    '    ts,',
    '    receivedAt,',
    '    retain: !!msg.retain,',
    '    qos: msg.qos === undefined ? null : msg.qos,',
    '    masterId,',
    '    payload: parsed,',
    '    rawJson: JSON.stringify(parsed)',
    '};',
    'return msg;'
);

const buildDeviceUpsertFunc = script(
    ...toSqlLines,
    ...pickStringLines,
    'const normalizeDeviceClass = (value) => {',
    '    if (typeof value !== "string" || !value.trim()) {',
    '        return null;',
    '    }',
    '    const normalized = value.trim().toLowerCase().replace(/[^a-z0-9]+/g, "_").replace(/^_+|_+$/g, "");',
    '    if (!normalized) {',
    '        return null;',
    '    }',
    '    if (normalized === "master" || normalized.startsWith("master_")) {',
    '        return "master";',
    '    }',
    '    if (normalized.startsWith("net_erl")) {',
    '        return "net_erl";',
    '    }',
    '    if (normalized.startsWith("net_zrl")) {',
    '        return "net_zrl";',
    '    }',
    '    if (normalized.startsWith("net_sen")) {',
    '        return "net_sen";',
    '    }',
    '    if (normalized.startsWith("bat_sen")) {',
    '        return "bat_sen";',
    '    }',
    '    return normalized;',
    '};',
    'const inferDeviceClassFromId = (value) => {',
    '    if (typeof value !== "string" || !value.trim()) {',
    '        return null;',
    '    }',
    '    return normalizeDeviceClass(value);',
    '};',
    'const normalizePowerSource = (rawValue, deviceClass) => {',
    '    if (typeof rawValue === "number") {',
    '        if (rawValue === 0) return "mains";',
    '        if (rawValue === 1) return "battery";',
    '    }',
    '    const text = typeof rawValue === "string" ? rawValue.trim().toLowerCase() : "";',
    '    if (["mains", "line", "wired", "ac"].includes(text)) {',
    '        return "mains";',
    '    }',
    '    if (["battery", "bat", "dc"].includes(text)) {',
    '        return "battery";',
    '    }',
    '    if (deviceClass === "bat_sen") {',
    '        return "battery";',
    '    }',
    '    if (deviceClass === "master" || /^net_/.test(deviceClass || "")) {',
    '        return "mains";',
    '    }',
    '    return null;',
    '};',
    'const inferStatus = (payload) => {',
    '    if (!payload || typeof payload !== "object") {',
    '        return null;',
    '    }',
    '    if (typeof payload.status === "string" && payload.status.trim()) {',
    '        return payload.status.trim().toLowerCase();',
    '    }',
    '    if (payload.online === true) {',
    '        return "online";',
    '    }',
    '    if (payload.online === false) {',
    '        return "offline";',
    '    }',
    '    if (payload.fault === true) {',
    '        return "fault";',
    '    }',
    '    return null;',
    '};',
    'const event = msg.payload || {};',
    'if ((event.channel === "event" || event.channel === "ack") && event.retain) {',
    '    node.warn("Skipping retained " + event.channel + " for registry updates on " + (event.topic || "unknown topic"));',
    '    return null;',
    '}',
    'msg.smarthomeEvent = event;',
    'const payload = event.payload || {};',
    'const now = event.ts || event.receivedAt || new Date().toISOString();',
    'const defaultName = event.entityId;',
    'const explicitName = pickString(payload.display_name, payload.device_name, payload.name, payload.label);',
    'const displayName = explicitName || defaultName;',
    'const inferredClass = normalizeDeviceClass(',
    '    pickString(',
    '        payload.device_class,',
    '        payload.device_type,',
    '        payload.hardware_type,',
    '        inferDeviceClassFromId(event.entityId),',
    '        event.scope === "master" ? "master" : null',
    '    )',
    ');',
    'const deviceClass = inferredClass || "unknown";',
    'const metaCacheKey = "device_meta_" + event.entityId;',
    'const cachedMeta = flow.get(metaCacheKey) || {};',
    'const deviceType = pickString(payload.device_type, payload.device_class, cachedMeta.deviceType, deviceClass);',
    'const room = pickString(payload.room, payload.room_slug, cachedMeta.room);',
    'const hardwareType = pickString(payload.hw_variant, payload.hardware_type, payload.device_type);',
    'const firmwareVersion = pickString(payload.fw_version, payload.firmware_version, payload.fw, payload.version);',
    'const protocolVersion = pickString(payload.protocol_version, payload.schema);',
    'const powerSource = normalizePowerSource(payload.power_class ?? payload.power_source ?? payload.power_type, inferredClass);',
    'const manufacturer = pickString(payload.manufacturer);',
    'const model = pickString(payload.model);',
    'const statusSummary = event.channel === "status" ? inferStatus(payload) : null;',
    'const originMasterId = event.scope === "node"',
    '    ? pickString(event.masterId, payload.origin_master_id, payload.master_id)',
    '    : null;',
    'const effectivePowerSource = powerSource || cachedMeta.powerSource || "unknown";',
    'flow.set(metaCacheKey, {',
    '    deviceType,',
    '    powerSource: effectivePowerSource,',
    '    room',
    '});',
    'msg.registryDevice = {',
    '    deviceId: event.entityId,',
    '    deviceRole: event.scope === "master" ? "master" : "node",',
    '    deviceClass,',
    '    deviceType,',
    '    powerSource: effectivePowerSource,',
    '    room',
    '};',
    'msg.topic = [',
    '    "INSERT INTO devices (device_id, device_role, display_name, default_name, origin_master_id, device_class, hardware_type, firmware_version, protocol_version, power_source, manufacturer, model, status, meta_json, created_at, updated_at, first_seen_at, last_seen_at, last_meta_at, last_status_at, last_state_at, last_event_at, last_ack_at)",',
    '    "VALUES (" + [',
    '        toSql(event.entityId),',
    '        toSql(event.scope === "master" ? "master" : "node"),',
    '        toSql(displayName),',
    '        toSql(defaultName),',
    '        toSql(originMasterId),',
    '        toSql(deviceClass),',
    '        toSql(hardwareType),',
    '        toSql(firmwareVersion),',
    '        toSql(protocolVersion),',
    '        toSql(powerSource),',
    '        toSql(manufacturer),',
    '        toSql(model),',
    '        toSql(statusSummary),',
    '        toSql(event.scope === "node" && event.channel === "meta" ? event.rawJson : null),',
    '        toSql(now),',
    '        toSql(now),',
    '        toSql(now),',
    '        toSql(now),',
    '        toSql(event.scope === "node" && event.channel === "meta" ? now : null),',
    '        toSql(event.channel === "status" ? now : null),',
    '        toSql(event.channel === "state" ? now : null),',
    '        toSql(event.channel === "event" ? now : null),',
    '        toSql(event.channel === "ack" ? now : null)',
    '    ].join(", ") + ")",',
    '    "ON CONFLICT(device_id) DO UPDATE SET",',
    '    "device_role = excluded.device_role,",',
    '    "updated_at = excluded.updated_at,",',
    '    "last_seen_at = excluded.last_seen_at,",',
    '    "display_name = CASE WHEN excluded.meta_json IS NOT NULL OR devices.display_name = devices.default_name THEN excluded.display_name ELSE devices.display_name END,",',
    '    "origin_master_id = COALESCE(excluded.origin_master_id, devices.origin_master_id),",',
    '    "device_class = COALESCE(excluded.device_class, devices.device_class),",',
    '    "hardware_type = COALESCE(excluded.hardware_type, devices.hardware_type),",',
    '    "firmware_version = COALESCE(excluded.firmware_version, devices.firmware_version),",',
    '    "protocol_version = COALESCE(excluded.protocol_version, devices.protocol_version),",',
    '    "power_source = COALESCE(excluded.power_source, devices.power_source),",',
    '    "manufacturer = COALESCE(excluded.manufacturer, devices.manufacturer),",',
    '    "model = COALESCE(excluded.model, devices.model),",',
    '    "status = CASE WHEN excluded.status IS NOT NULL THEN excluded.status ELSE devices.status END,",',
    '    "meta_json = CASE WHEN excluded.meta_json IS NOT NULL THEN excluded.meta_json ELSE devices.meta_json END,",',
    '    "last_meta_at = CASE WHEN excluded.last_meta_at IS NOT NULL THEN excluded.last_meta_at ELSE devices.last_meta_at END,",',
    '    "last_status_at = CASE WHEN excluded.last_status_at IS NOT NULL THEN excluded.last_status_at ELSE devices.last_status_at END,",',
    '    "last_state_at = CASE WHEN excluded.last_state_at IS NOT NULL THEN excluded.last_state_at ELSE devices.last_state_at END,",',
    '    "last_event_at = CASE WHEN excluded.last_event_at IS NOT NULL THEN excluded.last_event_at ELSE devices.last_event_at END,",',
    '    "last_ack_at = CASE WHEN excluded.last_ack_at IS NOT NULL THEN excluded.last_ack_at ELSE devices.last_ack_at END"',
    '].join(" ");',
    'return msg;'
);

const routePostRegistryFunc = script(
    'const event = msg.smarthomeEvent || {};',
    'const toCapabilities = event.scope === "node" && event.channel === "meta" ? msg : null;',
    'const toStateStore = ["status", "state", "event", "ack"].includes(event.channel) ? msg : null;',
    'const toConfigStore = event.scope === "node" && event.channel === "cfg/report" ? msg : null;',
    'const toTimeSeries = event.scope === "node" && event.channel === "state" ? msg : null;',
    'const toAutomation = event.scope === "node" && event.channel === "state" ? msg : null;',
    'return [toCapabilities, toStateStore, toConfigStore, toTimeSeries, toAutomation];'
);

const buildCapabilityUpsertsFunc = script(
    ...toSqlLines,
    ...pickStringLines,
    'const inferRole = (key) => {',
    '    if (/relay|switch|cover|light|output/i.test(key)) {',
    '        return "actuator";',
    '    }',
    '    if (/temp|humid|lux|co2|eco2|tvoc|aqi|motion|contact|battery|button|rain/i.test(key)) {',
    '        return "sensor";',
    '    }',
    '    return "unknown";',
    '};',
    'const inferType = (key) => {',
    '    if (/relay|switch|motion|contact|fault|online|button|cover_calibrated/i.test(key)) {',
    '        return "boolean";',
    '    }',
    '    if (/temp|humid|lux|co2|eco2|tvoc|aqi|battery|position|rain|timeout|delay|interval|threshold/i.test(key)) {',
    '        return "number";',
    '    }',
    '    return null;',
    '};',
    'const inferUnit = (key) => {',
    '    if (/temperature|temp|_c$/i.test(key)) {',
    '        return "c";',
    '    }',
    '    if (/humidity|humid|_pct$/i.test(key)) {',
    '        return "pct";',
    '    }',
    '    if (/lux/i.test(key)) {',
    '        return "lux";',
    '    }',
    '    if (/co2|eco2/i.test(key)) {',
    '        return "ppm";',
    '    }',
    '    if (/tvoc/i.test(key)) {',
    '        return "ppb";',
    '    }',
    '    return null;',
    '};',
    'const capabilityMaskMap = {',
    '    0x0001: "relay_1",',
    '    0x0002: "relay_2",',
    '    0x0004: "temperature_c",',
    '    0x0008: "humidity_pct",',
    '    0x0010: "lux",',
    '    0x0020: "aqi",',
    '    0x0040: "motion",',
    '    0x0080: "contact",',
    '    0x0100: "rain",',
    '    0x0200: "battery_pct",',
    '    0x0400: "button_1",',
    '    0x0800: "buttons",',
    '    0x1000: "led_ring",',
    '    0x2000: "cover",',
    '    0x4000: "setup_portal"',
    '};',
    'const event = msg.smarthomeEvent || msg.payload || {};',
    'const payload = event.payload || {};',
    'if (event.scope !== "node" || event.channel !== "meta") {',
    '    return null;',
    '}',
    'let capabilities = [];',
    'if (Array.isArray(payload.capabilities)) {',
    '    capabilities = payload.capabilities;',
    '} else if (payload.capabilities && typeof payload.capabilities === "object") {',
    '    capabilities = Object.entries(payload.capabilities).map(([key, meta]) => (meta && typeof meta === "object") ? { key, ...meta } : { key });',
    '} else {',
    '    let mask = null;',
    '    if (typeof payload.capability_mask === "number" && Number.isFinite(payload.capability_mask)) {',
    '        mask = payload.capability_mask;',
    '    } else if (typeof payload.capability_mask === "string" && payload.capability_mask.trim()) {',
    '        const parsedMask = Number(payload.capability_mask);',
    '        if (Number.isFinite(parsedMask)) {',
    '            mask = parsedMask;',
    '        }',
    '    } else if (Number.isFinite(payload.caps_hi) || Number.isFinite(payload.caps_lo)) {',
    '        const hi = Number.isFinite(payload.caps_hi) ? Number(payload.caps_hi) : 0;',
    '        const lo = Number.isFinite(payload.caps_lo) ? Number(payload.caps_lo) : 0;',
    '        mask = ((hi & 0xFF) << 8) | (lo & 0xFF);',
    '    }',
    '    if (mask !== null) {',
    '        capabilities = Object.entries(capabilityMaskMap)',
    '            .filter(([bit]) => (mask & Number(bit)) !== 0)',
    '            .map(([, key]) => ({ key }));',
    '    }',
    '}',
    'const now = event.ts || event.receivedAt || new Date().toISOString();',
    'const messages = [];',
    'for (const capability of capabilities) {',
    '    const key = typeof capability === "string"',
    '        ? capability',
    '        : capability && typeof capability === "object"',
    '            ? pickString(capability.key, capability.id, capability.name)',
    '            : null;',
    '    if (!key) {',
    '        continue;',
    '    }',
    '    const meta = capability && typeof capability === "object" ? capability : { key };',
    '    messages.push({',
    '        smarthomeEvent: event,',
    '        topic: [',
    '            "INSERT INTO device_capabilities (device_id, capability_key, capability_role, data_type, unit, writable, readable, channel, meta_json, created_at, updated_at)",',
    '            "VALUES (" + [',
    '                toSql(event.entityId),',
    '                toSql(key),',
    '                toSql(meta.role || inferRole(key)),',
    '                toSql(meta.data_type || meta.type || inferType(key)),',
    '                toSql(meta.unit || inferUnit(key)),',
    '                toSql(meta.writable === undefined ? (/relay|switch|cover|light|output/i.test(key) ? 1 : 0) : (meta.writable ? 1 : 0)),',
    '                toSql(meta.readable === undefined ? 1 : (meta.readable ? 1 : 0)),',
    '                toSql(meta.channel || null),',
    '                toSql(JSON.stringify(meta)),',
    '                toSql(now),',
    '                toSql(now)',
    '            ].join(", ") + ")",',
    '            "ON CONFLICT(device_id, capability_key) DO UPDATE SET",',
    '            "capability_role = excluded.capability_role,",',
    '            "data_type = COALESCE(excluded.data_type, device_capabilities.data_type),",',
    '            "unit = COALESCE(excluded.unit, device_capabilities.unit),",',
    '            "writable = excluded.writable,",',
    '            "readable = excluded.readable,",',
    '            "channel = COALESCE(excluded.channel, device_capabilities.channel),",',
    '            "meta_json = excluded.meta_json,",',
    '            "updated_at = excluded.updated_at"',
    '        ].join(" ")',
    '    });',
    '}',
    'return messages.length ? [messages] : null;'
);

const buildLastStateUpsertFunc = script(
    ...toSqlLines,
    'const event = msg.smarthomeEvent || msg.payload || {};',
    'const now = event.ts || event.receivedAt || new Date().toISOString();',
    'if ((event.channel === "event" || event.channel === "ack") && event.retain) {',
    '    node.warn("Ignoring retained " + event.channel + " for current-state storage on " + event.topic);',
    '    return null;',
    '}',
    'if (event.channel === "status") {',
    '    msg.topic = [',
    '        "INSERT INTO device_last_state (device_id, status_json, status_at, updated_at)",',
    '        "VALUES (" + [toSql(event.entityId), toSql(event.rawJson), toSql(now), toSql(now)].join(", ") + ")",',
    '        "ON CONFLICT(device_id) DO UPDATE SET",',
    '        "status_json = excluded.status_json,",',
    '        "status_at = excluded.status_at,",',
    '        "updated_at = excluded.updated_at"',
    '    ].join(" ");',
    '    return msg;',
    '}',
    'if (event.channel === "state") {',
    '    msg.topic = [',
    '        "INSERT INTO device_last_state (device_id, state_json, state_at, updated_at)",',
    '        "VALUES (" + [toSql(event.entityId), toSql(event.rawJson), toSql(now), toSql(now)].join(", ") + ")",',
    '        "ON CONFLICT(device_id) DO UPDATE SET",',
    '        "state_json = excluded.state_json,",',
    '        "state_at = excluded.state_at,",',
    '        "updated_at = excluded.updated_at"',
    '    ].join(" ");',
    '    return msg;',
    '}',
    'if (event.channel === "event") {',
    '    msg.topic = [',
    '        "INSERT INTO device_last_state (device_id, event_json, event_at, updated_at)",',
    '        "VALUES (" + [toSql(event.entityId), toSql(event.rawJson), toSql(now), toSql(now)].join(", ") + ")",',
    '        "ON CONFLICT(device_id) DO UPDATE SET",',
    '        "event_json = excluded.event_json,",',
    '        "event_at = excluded.event_at,",',
    '        "updated_at = excluded.updated_at"',
    '    ].join(" ");',
    '    return msg;',
    '}',
    'if (event.channel === "ack") {',
    '    msg.topic = [',
    '        "INSERT INTO device_last_state (device_id, ack_json, ack_at, updated_at)",',
    '        "VALUES (" + [toSql(event.entityId), toSql(event.rawJson), toSql(now), toSql(now)].join(", ") + ")",',
    '        "ON CONFLICT(device_id) DO UPDATE SET",',
    '        "ack_json = excluded.ack_json,",',
    '        "ack_at = excluded.ack_at,",',
    '        "updated_at = excluded.updated_at"',
    '    ].join(" ");',
    '    return msg;',
    '}',
    'return null;'
);

const buildServerStatusUpsertsFunc = script(
    ...toSqlLines,
    'const event = msg.payload || {};',
    'if (event.scope !== "server" || event.channel !== "status") {',
    '    return null;',
    '}',
    'const now = event.ts || event.receivedAt || new Date().toISOString();',
    'const messages = [',
    '    {',
    '        topic: [',
    '            "INSERT INTO server_settings (setting_key, setting_value, value_type, updated_at)",',
    '            "VALUES (" + [toSql("server_status_json"), toSql(event.rawJson), toSql("json"), toSql(now)].join(", ") + ")",',
    '            "ON CONFLICT(setting_key) DO UPDATE SET",',
    '            "setting_value = excluded.setting_value,",',
    '            "value_type = excluded.value_type,",',
    '            "updated_at = excluded.updated_at"',
    '        ].join(" ")',
    '    },',
    '    {',
    '        topic: [',
    '            "INSERT INTO server_settings (setting_key, setting_value, value_type, updated_at)",',
    '            "VALUES (" + [toSql("server_status_at"), toSql(now), toSql("datetime"), toSql(now)].join(", ") + ")",',
    '            "ON CONFLICT(setting_key) DO UPDATE SET",',
    '            "setting_value = excluded.setting_value,",',
    '            "value_type = excluded.value_type,",',
    '            "updated_at = excluded.updated_at"',
    '        ].join(" ")',
    '    }',
    '];',
    'return [messages];'
);

const buildConfigUpsertsFunc = script(
    ...toSqlLines,
    ...pickStringLines,
    'const cfgKeyByParamId = {',
    '    1: "device_name",',
    '    2: "report_interval_s",',
    '    3: "ack_timeout_ms",',
    '    4: "max_retries",',
    '    5: "event_debounce_ms",',
    '    6: "led_enabled",',
    '    16: "temp_report_delta",',
    '    17: "hum_report_delta",',
    '    18: "lux_report_delta",',
    '    19: "presence_hold_s",',
    '    32: "relay_mode",',
    '    33: "auto_off_delay_s",',
    '    34: "light_threshold_on",',
    '    35: "light_threshold_off",',
    '    36: "relay_default_on_boot",',
    '    48: "cover_run_time_up_ms",',
    '    49: "cover_run_time_down_ms",',
    '    50: "cover_reverse_lock_ms",',
    '    51: "cover_calibrated",',
    '    64: "wake_interval_s",',
    '    65: "rx_window_ms",',
    '    66: "low_battery_threshold",',
    '    80: "ring_enabled",',
    '    81: "ring_brightness",',
    '    82: "ring_mode"',
    '};',
    'const event = msg.smarthomeEvent || msg.payload || {};',
    'const payload = event.payload || {};',
    'if (event.scope !== "node" || event.channel !== "cfg/report") {',
    '    return null;',
    '}',
    'const now = event.ts || event.receivedAt || new Date().toISOString();',
    'const metaKeys = new Set([',
    '    "schema", "ts", "timestamp", "reported_at", "device_id", "node_id", "master_id",',
    '    "display_name", "device_name", "device_class", "device_type", "hw_variant", "fw",',
    '    "fw_version", "firmware_version", "protocol_version", "source", "status", "profile_id",',
    '    "revision", "config_revision", "values", "config", "reported", "params", "entries"',
    ']);',
    'const entries = new Map();',
    'const setEntry = (key, value) => {',
    '    if (typeof key !== "string" || !key.trim()) {',
    '        return;',
    '    }',
    '    entries.set(key.trim(), value);',
    '};',
    'const addObjectEntries = (objectValue) => {',
    '    if (!objectValue || typeof objectValue !== "object" || Array.isArray(objectValue)) {',
    '        return;',
    '    }',
    '    for (const [key, value] of Object.entries(objectValue)) {',
    '        if (metaKeys.has(key)) {',
    '            continue;',
    '        }',
    '        setEntry(key, value);',
    '    }',
    '};',
    'const addArrayEntries = (arrayValue) => {',
    '    if (!Array.isArray(arrayValue)) {',
    '        return;',
    '    }',
    '    for (const item of arrayValue) {',
    '        if (!item || typeof item !== "object") {',
    '            continue;',
    '        }',
    '        const namedKey = pickString(item.key, item.id, item.name);',
    '        if (namedKey) {',
    '            setEntry(namedKey, item.value);',
    '            continue;',
    '        }',
    '        if (Number.isFinite(item.param_id)) {',
    '            const mappedKey = cfgKeyByParamId[item.param_id] || ("param_" + item.param_id);',
    '            setEntry(mappedKey, item.value);',
    '        }',
    '    }',
    '};',
    'if (Array.isArray(payload.values)) {',
    '    addArrayEntries(payload.values);',
    '} else if (payload.values && typeof payload.values === "object") {',
    '    addObjectEntries(payload.values);',
    '}',
    'if (payload.config && typeof payload.config === "object") {',
    '    addObjectEntries(payload.config);',
    '}',
    'if (payload.reported && typeof payload.reported === "object") {',
    '    addObjectEntries(payload.reported);',
    '}',
    'if (Array.isArray(payload.params)) {',
    '    addArrayEntries(payload.params);',
    '}',
    'if (Array.isArray(payload.entries)) {',
    '    addArrayEntries(payload.entries);',
    '}',
    'if (!entries.size) {',
    '    addObjectEntries(payload);',
    '}',
    'const messages = [];',
    'for (const [configKey, rawValue] of entries.entries()) {',
    '    const configType = rawValue === null ? "null"',
    '        : Array.isArray(rawValue) || (rawValue && typeof rawValue === "object") ? "json"',
    '        : typeof rawValue;',
    '    const serializedValue = rawValue === null || rawValue === undefined',
    '        ? null',
    '        : typeof rawValue === "string"',
    '            ? rawValue',
    '            : JSON.stringify(rawValue);',
    '    messages.push({',
    '        smarthomeEvent: event,',
    '        topic: [',
    '            "INSERT INTO device_config (device_id, config_key, config_value, config_type, source_channel, last_reported_at, updated_at)",',
    '            "VALUES (" + [',
    '                toSql(event.entityId),',
    '                toSql(configKey),',
    '                toSql(serializedValue),',
    '                toSql(configType),',
    '                toSql("cfg/report"),',
    '                toSql(now),',
    '                toSql(now)',
    '            ].join(", ") + ")",',
    '            "ON CONFLICT(device_id, config_key) DO UPDATE SET",',
    '            "config_value = excluded.config_value,",',
    '            "config_type = excluded.config_type,",',
    '            "source_channel = excluded.source_channel,",',
    '            "last_reported_at = excluded.last_reported_at,",',
    '            "updated_at = excluded.updated_at"',
    '        ].join(" ")',
    '    });',
    '};',
    'return messages.length ? [messages] : null;'
);

const buildInfluxLineFunc = script(
    'const event = msg.smarthomeEvent || msg.payload || {};',
    'if (event.scope !== "node" || event.channel !== "state") {',
    '    return null;',
    '}',
    'const payload = event.payload || {};',
    'const sensorRoot = payload.sensors;',
    'const escapeTag = (value) => String(value).replace(/ /g, "\\\\ ").replace(/,/g, "\\\\,").replace(/=/g, "\\\\=");',
    'const normalizeMetric = (value) => String(value)',
    '    .trim()',
    '    .toLowerCase()',
    '    .replace(/[^a-z0-9_]+/g, "_")',
    '    .replace(/^_+|_+$/g, "");',
    'const inferUnit = (metric) => {',
    '    if (/_01c$/i.test(metric)) return "0.1c";',
    '    if (/_01pct$/i.test(metric)) return "0.1pct";',
    '    if (/temp|_c$/i.test(metric)) return "c";',
    '    if (/hum|humid|_pct$|pct$/i.test(metric)) return "pct";',
    '    if (/eco2|co2/i.test(metric)) return "ppm";',
    '    if (/tvoc|voc/i.test(metric)) return "ppb";',
    '    if (/lux/i.test(metric)) return "lux";',
    '    if (/battery_pct/i.test(metric)) return "pct";',
    '    if (/battery_mv|_mv$/i.test(metric)) return "mv";',
    '    if (/battery_v|_v$|volt/i.test(metric)) return "v";',
    '    return null;',
    '};',
    'const isTimeSeriesMetric = (metric) => {',
    '    if (!metric) return false;',
    '    if (/relay|switch|cover|output|fault|online|status|event|command|motion|contact|window/i.test(metric)) return false;',
    '    return /(temp|hum|humid|co2|eco2|tvoc|voc|lux|battery(_mv|_v|_pct)?|volt|pressure|current|power|energy|aqi|rain|water|flow|pm\\d*|soil|moist|uv)/i.test(metric);',
    '};',
    'const registryDevice = msg.registryDevice || {};',
    'const nodeId = event.entityId || event.deviceId;',
    'if (!nodeId) {',
    '    return null;',
    '}',
    'const points = [];',
    'const appendPoint = (metric, value, unit) => {',
    '    if (typeof value !== "number" || !Number.isFinite(value)) return;',
    '    const normalizedMetric = normalizeMetric(metric);',
    '    if (!normalizedMetric || !isTimeSeriesMetric(normalizedMetric)) return;',
    '    points.push({ metric: normalizedMetric, unit: typeof unit === "string" && unit ? unit : inferUnit(normalizedMetric), value: Number(value) });',
    '};',
    'const visitSensors = (value, prefix) => {',
    '    if (value === null || value === undefined) return;',
    '    if (typeof value === "number") {',
    '        appendPoint(prefix, value, null);',
    '        return;',
    '    }',
    '    if (Array.isArray(value) || typeof value !== "object") return;',
    '    if (typeof value.value === "number" && Number.isFinite(value.value)) {',
    '        appendPoint(prefix, value.value, value.unit || value.uom || null);',
    '        return;',
    '    }',
    '    for (const [key, current] of Object.entries(value)) {',
    '        const nextPrefix = prefix ? prefix + "_" + key : key;',
    '        visitSensors(current, nextPrefix);',
    '    }',
    '};',
    'if (sensorRoot && typeof sensorRoot === "object" && !Array.isArray(sensorRoot)) {',
    '    visitSensors(sensorRoot, "");',
    '} else {',
    '    for (const [key, current] of Object.entries(payload)) {',
    '        if (typeof current === "number") {',
    '            appendPoint(key, current, null);',
    '            continue;',
    '        }',
    '        if (current && typeof current === "object" && !Array.isArray(current) && typeof current.value === "number" && Number.isFinite(current.value)) {',
    '            appendPoint(key, current.value, current.unit || current.uom || null);',
    '        }',
    '    }',
    '}',
    'if (!points.length) {',
    '    return null;',
    '}',
    'const tags = ["node_id=" + escapeTag(nodeId)];',
    'const deviceType = registryDevice.deviceType',
    '    || (typeof payload.device_type === "string" && payload.device_type ? payload.device_type : null)',
    '    || registryDevice.deviceClass',
    '    || (typeof payload.device_class === "string" && payload.device_class ? payload.device_class : null);',
    'if (deviceType) {',
    '    tags.push("device_type=" + escapeTag(deviceType));',
    '}',
    'const room = registryDevice.room',
    '    || (typeof payload.room === "string" && payload.room ? payload.room : null)',
    '    || (typeof payload.room_slug === "string" && payload.room_slug ? payload.room_slug : null);',
    'if (room) {',
    '    tags.push("room=" + escapeTag(room));',
    '}',
    'const timestampMs = Date.parse(event.ts || event.receivedAt || new Date().toISOString());',
    'const safeTimestampMs = Number.isNaN(timestampMs) ? Date.now() : timestampMs;',
    'const timestampNs = (BigInt(safeTimestampMs) * 1000000n).toString();',
    'const influxToken = env.get("INFLUX_TOKEN");',
    'if (!influxToken) {',
    '    node.warn("Skipping Influx write because INFLUX_TOKEN is empty.");',
    '    return null;',
    '}',
    'msg.url = (env.get("INFLUX_URL") || "http://influxdb:8086")',
    '    + "/api/v2/write?org=" + encodeURIComponent(env.get("INFLUX_ORG") || "")',
    '    + "&bucket=" + encodeURIComponent(env.get("INFLUX_BUCKET") || "smarthome")',
    '    + "&precision=ns";',
    'msg.headers = {',
    '    Authorization: "Token " + influxToken,',
    '    "Content-Type": "text/plain; charset=utf-8"',
    '};',
    'msg.payload = points.map((point) => {',
    '    const pointTags = tags.slice();',
    '    pointTags.push("metric=" + escapeTag(point.metric));',
    '    if (point.unit) pointTags.push("unit=" + escapeTag(point.unit));',
    '    return "smarthome_sensor," + pointTags.join(",") + " value_num=" + String(point.value) + " " + timestampNs;',
    '}).join("\\n");',
    'return msg;'
);

const buildNetErlCmdSetFunc = script(
    'const relayState = msg.payload === true || msg.payload === "true" || msg.payload === 1 || msg.payload === "1";',
    'const stateLabel = relayState ? "on" : "off";',
    'const occurredAt = new Date().toISOString();',
    'const topic = "smarthome/node/net_erl_01/cmd/set";',
    'const commandPayload = {',
    '    cmd: "set_relay",',
    '    relay_1: relayState,',
    '    request_id: "nodered_net_erl_01_" + stateLabel + "_" + Date.now()',
    '};',
    'const rawJson = JSON.stringify(commandPayload);',
    'const publishMsg = {',
    '    topic,',
    '    payload: rawJson',
    '};',
    'const auditMsg = {',
    '    payload: {',
    '        topic,',
    '        scope: "node",',
    '        entityId: "net_erl_01",',
    '        channel: "cmd/set",',
    '        direction: "egress",',
    '        retain: false,',
    '        ts: occurredAt,',
    '        receivedAt: occurredAt,',
    '        payload: commandPayload,',
    '        rawJson',
    '    }',
    '};',
    'return [publishMsg, auditMsg];'
);

const buildNetZrlCmdSetFunc = script(
    'const relayKey = msg.topic === "relay_2" ? "relay_2" : "relay_1";',
    'const relayState = msg.payload === true || msg.payload === "true" || msg.payload === 1 || msg.payload === "1";',
    'const stateLabel = relayState ? "on" : "off";',
    'const occurredAt = new Date().toISOString();',
    'const topic = "smarthome/node/net_zrl_01/cmd/set";',
    'const commandPayload = {',
    '    cmd: "set_relay",',
    '    [relayKey]: relayState,',
    '    request_id: "nodered_net_zrl_01_" + relayKey + "_" + stateLabel + "_" + Date.now()',
    '};',
    'const rawJson = JSON.stringify(commandPayload);',
    'const publishMsg = {',
    '    topic,',
    '    payload: rawJson',
    '};',
    'const auditMsg = {',
    '    payload: {',
    '        topic,',
    '        scope: "node",',
    '        entityId: "net_zrl_01",',
    '        channel: "cmd/set",',
    '        direction: "egress",',
    '        retain: false,',
    '        ts: occurredAt,',
    '        receivedAt: occurredAt,',
    '        payload: commandPayload,',
    '        rawJson',
    '    }',
    '};',
    'return [publishMsg, auditMsg];'
);

const buildAuditInsertFunc = script(
    ...toSqlLines,
    'const event = msg.payload || {};',
    'const direction = typeof event.direction === "string" && event.direction.trim() ? event.direction.trim() : "ingest";',
    'msg.topic = [',
    '    "INSERT INTO audit_log (topic, device_id, master_id, scope, channel, direction, retain, payload_json, occurred_at)",',
    '    "VALUES (" + [',
    '        toSql(event.topic),',
    '        toSql(event.scope === "server" ? null : event.entityId),',
    '        toSql(event.scope === "master" ? event.entityId : event.masterId),',
    '        toSql(event.scope || "unknown"),',
    '        toSql(event.channel || "unknown"),',
    '        toSql(direction),',
    '        toSql(event.retain ? 1 : 0),',
    '        toSql(event.rawJson || null),',
    '        toSql(event.ts || event.receivedAt || new Date().toISOString())',
    '    ].join(", ") + ")"',
    '].join(" ");',
    'return msg;'
);

const routeSimDashboardRefreshFunc = String.raw`
const pagePath = "/simulation";

if (msg.topic === "$pageview") {
    const payload = msg.payload || {};
    const page = payload.page || {};
    const currentPath = typeof page.path === "string" ? page.path : "";
    if (!currentPath || !currentPath.endsWith(pagePath)) {
        return null;
    }
}

return msg;
`;

const buildSimDashboardQueryFunc = String.raw`
msg.topic = [
    "SELECT",
    "    d.device_id,",
    "    d.device_role,",
    "    d.display_name,",
    "    d.origin_master_id,",
    "    d.device_class,",
    "    d.power_source,",
    "    d.last_seen_at,",
    "    d.last_meta_at,",
    "    d.last_status_at,",
    "    d.last_state_at,",
    "    ls.status_json,",
    "    ls.state_json,",
    "    d.meta_json",
    "FROM devices AS d",
    "LEFT JOIN device_last_state AS ls ON ls.device_id = d.device_id",
    "WHERE d.device_id GLOB 'sim_*'",
    "ORDER BY CASE WHEN d.device_role = 'master' THEN 0 ELSE 1 END, d.device_id;"
].join(" ");

return msg;
`;

const buildSimDashboardMarkdownFunc = String.raw`
const parseJson = (value) => {
    if (!value) {
        return null;
    }
    try {
        return JSON.parse(value);
    } catch (error) {
        return {
            _parse_error: error.message,
            _raw: value
        };
    }
};

const asText = (value) => {
    if (value === null || value === undefined || value === "") {
        return "-";
    }
    return String(value);
};

const asBool = (value) => {
    if (value === true) {
        return "true";
    }
    if (value === false) {
        return "false";
    }
    return "-";
};

const asNumber = (value, unit) => {
    if (typeof value !== "number" || !Number.isFinite(value)) {
        return "-";
    }
    return unit ? String(value) + " " + unit : String(value);
};

const asScaled = (value, divisor, unit) => {
    if (typeof value !== "number" || !Number.isFinite(value)) {
        return "-";
    }
    return (value / divisor).toFixed(1) + " " + unit;
};

const jsonBlock = (lines, title, value) => {
    lines.push(title + ":");
    if (!value) {
        lines.push("(kein Eintrag)");
        lines.push("");
        return;
    }
    lines.push(JSON.stringify(value, null, 2));
    lines.push("");
};

const ageSeconds = (isoValue) => {
    if (!isoValue) {
        return null;
    }
    const age = Math.floor((Date.now() - Date.parse(isoValue)) / 1000);
    if (!Number.isFinite(age)) {
        return null;
    }
    return Math.max(age, 0);
};

const ageLabel = (seconds) => {
    if (seconds === null || seconds === undefined) {
        return "-";
    }
    if (seconds < 60) {
        return seconds + " s";
    }
    if (seconds < 3600) {
        const minutes = Math.floor(seconds / 60);
        const restSeconds = seconds % 60;
        return minutes + " min " + restSeconds + " s";
    }
    const hours = Math.floor(seconds / 3600);
    const minutes = Math.floor((seconds % 3600) / 60);
    return hours + " h " + minutes + " min";
};

const keyLines = (deviceId, state) => {
    const lines = [];
    if (deviceId === "sim_net_sen_01") {
        lines.push("Temperatur: " + asScaled(state && state.temp_01c, 10, "C"));
        lines.push("Feuchte: " + asScaled(state && state.hum_01pct, 10, "%"));
        lines.push("Lux: " + asNumber(state && state.lux, ""));
        lines.push("Motion: " + asBool(state && state.motion));
    } else if (deviceId === "sim_bat_sen_01") {
        lines.push("Batterie: " + asNumber(state && state.battery_mv, "mV"));
        lines.push("Batterie-Prozent: " + asNumber(state && state.battery_pct, "%"));
    } else if (deviceId === "sim_net_erl_01") {
        lines.push("relay_1: " + asBool(state && state.relay_1));
        lines.push("fault: " + asBool(state && state.fault));
    } else if (deviceId === "sim_net_zrl_01") {
        lines.push("relay_1: " + asBool(state && state.relay_1));
        lines.push("relay_2: " + asBool(state && state.relay_2));
        lines.push("cover_mode: " + asBool(state && state.cover_mode));
        lines.push("fault: " + asBool(state && state.fault));
    }
    return lines;
};

const tableRow = (cells) => {
    const widths = [18, 8, 10, 10, 24, 10, 18, 16];
    return cells
        .map((cell, index) => asText(cell).padEnd(widths[index], " "))
        .join(" | ");
};

const rows = Array.isArray(msg.payload) ? msg.payload : [];
const devices = rows
    .map((row) => {
        const meta = parseJson(row.meta_json);
        const status = parseJson(row.status_json);
        const state = parseJson(row.state_json);
        const online = status && typeof status.online === "boolean" ? status.online : null;
        const seenAge = ageSeconds(row.last_seen_at);
        return {
            id: row.device_id,
            role: row.device_role || "node",
            displayName: row.display_name || row.device_id,
            originMasterId: row.origin_master_id || "-",
            deviceClass: row.device_class || "-",
            powerSource: row.power_source || "-",
            lastSeenAt: row.last_seen_at || "-",
            lastSeenAge: ageLabel(seenAge),
            lastMetaAt: row.last_meta_at || "-",
            lastStatusAt: row.last_status_at || "-",
            lastStateAt: row.last_state_at || "-",
            onlineLabel: online === true ? "online" : online === false ? "offline" : "unbekannt",
            staleLabel: seenAge !== null && seenAge > 120 ? "moeglich veraltet" : "-",
            meta,
            status,
            state,
            keyLines: keyLines(row.device_id, state)
        };
    })
    .sort((left, right) => {
        if (left.role !== right.role) {
            return left.role === "master" ? -1 : 1;
        }
        return left.id.localeCompare(right.id);
    });

const byId = {};
devices.forEach((device) => {
    byId[device.id] = device;
});

const onlineCount = devices.filter((device) => device.onlineLabel === "online").length;
const offlineCount = devices.filter((device) => device.onlineLabel === "offline").length;
const lines = [];

lines.push("[SIM] Server-Validierung");
lines.push("========================");
lines.push("");
lines.push("Nur sim_* IDs. Kein Hardware-, Funk- oder HELLO-Nachweis.");
lines.push("");
lines.push("Stand: " + new Date().toISOString());
lines.push("Registrierte sim_* Geraete: " + String(devices.length));
lines.push("Online: " + String(onlineCount));
lines.push("Offline: " + String(offlineCount));
lines.push("Diese Seite mischt bewusst keine realen Pilot-IDs bei.");
lines.push("");

if (!devices.length) {
    lines.push("Keine sim_* Geraete in SQLite sichtbar.");
    lines.push("");
    lines.push("Starte zuerst den isolierten Stack smarthome_sim und fahre danach den Fake-Master-Harness.");
    msg.payload = lines.join("\n");
    return msg;
}

lines.push("Registrierung / Presence");
lines.push("------------------------");
lines.push("");
lines.push(tableRow(["Device", "Rolle", "Klasse", "Online", "Last seen", "Alter", "Hinweis", "Master"]));
lines.push(tableRow(["------------------", "--------", "----------", "----------", "------------------------", "----------", "------------------", "----------------"]));
devices.forEach((device) => {
    lines.push(tableRow([
        device.id,
        device.role,
        device.deviceClass,
        device.onlineLabel,
        device.lastSeenAt,
        device.lastSeenAge,
        device.staleLabel,
        device.originMasterId
    ]));
});
lines.push("");

const pushDetailSection = (deviceId, title) => {
    const device = byId[deviceId];
    lines.push(title);
    lines.push("-".repeat(title.length));
    lines.push("");
    if (!device) {
        lines.push("Noch nicht registriert.");
        lines.push("");
        return;
    }
    lines.push("Anzeige: " + device.displayName);
    lines.push("Online: " + device.onlineLabel);
    lines.push("Last seen: " + device.lastSeenAt + " (" + device.lastSeenAge + ")");
    lines.push("last_meta_at: " + device.lastMetaAt);
    lines.push("last_status_at: " + device.lastStatusAt);
    lines.push("last_state_at: " + device.lastStateAt);
    device.keyLines.forEach((line) => {
        lines.push(line);
    });
    lines.push("");
    jsonBlock(lines, "meta", device.meta);
    jsonBlock(lines, "status", device.status);
    jsonBlock(lines, "state", device.state);
};

pushDetailSection("sim_master_01", "sim_master_01");
pushDetailSection("sim_net_sen_01", "sim_net_sen_01");
pushDetailSection("sim_bat_sen_01", "sim_bat_sen_01");
pushDetailSection("sim_net_erl_01", "sim_net_erl_01");
pushDetailSection("sim_net_zrl_01", "sim_net_zrl_01");

msg.payload = lines.join("\n");
return msg;
`;

const simDashboardTemplate = String.raw`
<template>
    <div class="sim-dashboard-template">
        <pre>{{ msg?.payload || "Noch keine Simulationsdaten geladen." }}</pre>
    </div>
</template>

<style>
    .sim-dashboard-template {
        padding: 0.25rem 0;
    }

    .sim-dashboard-template pre {
        margin: 0;
        white-space: pre-wrap;
        word-break: break-word;
        font-family: "Courier New", Courier, monospace;
        font-size: 0.84rem;
        line-height: 1.45;
    }
</style>
`;

const renderDashboardNavHtml = () => "";

const dashboardSharedStyle = String.raw`
<style>
    .sh-page {
        --sh-surface-base: #091017;
        --sh-surface-panel: #101822;
        --sh-surface-card: #15202b;
        --sh-surface-card-strong: #182736;
        --sh-surface-input: #0d141c;
        --sh-border: #243445;
        --sh-border-soft: #1c2a37;
        --sh-text: #ecf3fa;
        --sh-text-muted: #9db0c3;
        --sh-chip-bg: #182432;
        --sh-chip-text: #dce8f4;
        padding: 0.35rem 0 0.65rem;
        color: var(--sh-text);
    }

    .sh-page * {
        box-sizing: border-box;
    }

    .sh-toolbar {
        display: flex;
        gap: 0.65rem;
        justify-content: flex-end;
        align-items: center;
        flex-wrap: wrap;
        margin-bottom: 0.6rem;
    }

    .sh-toolbar h2 {
        margin: 0;
        font-size: 1.2rem;
        line-height: 1.2;
    }

    .sh-toolbar p,
    .sh-muted {
        margin: 0.2rem 0 0;
        color: var(--sh-text-muted);
        font-size: 0.92rem;
    }

    .sh-toolbar-actions {
        display: flex;
        gap: 0.45rem;
        flex-wrap: wrap;
        justify-content: flex-end;
        margin-left: auto;
    }

    .sh-nav-link,
    .sh-btn {
        display: inline-flex;
        align-items: center;
        gap: 0.3rem;
        padding: 0.52rem 0.82rem;
        border-radius: 999px;
        border: 1px solid var(--sh-border);
        background: linear-gradient(180deg, rgba(22, 33, 45, 0.95), rgba(11, 19, 27, 0.98));
        color: var(--sh-text);
        text-decoration: none;
        font-size: 0.88rem;
        line-height: 1;
        cursor: pointer;
        transition: transform 0.15s ease, border-color 0.15s ease, box-shadow 0.15s ease, background 0.15s ease;
    }

    .sh-nav-link:hover,
    .sh-btn:hover {
        transform: translateY(-1px);
        border-color: #36516b;
        box-shadow: 0 10px 18px rgba(0, 0, 0, 0.18);
    }

    .sh-btn-muted {
        background: linear-gradient(180deg, rgba(30, 41, 54, 0.92), rgba(18, 27, 37, 0.98));
    }

    .sh-btn-danger {
        border-color: #6a3a3a;
        background: #211517;
        color: #ffb9b9;
    }

    .sh-status-row {
        display: flex;
        gap: 0.5rem;
        flex-wrap: wrap;
        margin-bottom: 0.8rem;
    }

    .sh-chip {
        display: inline-flex;
        align-items: center;
        gap: 0.25rem;
        padding: 0.34rem 0.62rem;
        border-radius: 999px;
        background: var(--sh-chip-bg);
        color: var(--sh-chip-text);
        font-size: 0.82rem;
        line-height: 1;
        border: 1px solid transparent;
    }

    .sh-chip-online {
        background: rgba(21, 78, 53, 0.82);
        color: #9af3c2;
        border-color: rgba(86, 185, 129, 0.3);
    }

    .sh-chip-offline {
        background: rgba(86, 28, 38, 0.86);
        color: #ffb5bf;
        border-color: rgba(185, 89, 105, 0.28);
    }

    .sh-chip-unknown {
        background: #232e39;
        color: #b6c2cc;
    }

    .sh-chip-sim {
        background: rgba(66, 50, 19, 0.88);
        color: #ffd98a;
    }

    .sh-chip-power {
        background: rgba(25, 49, 76, 0.92);
        color: #a9d2ff;
    }

    .sh-weather-card,
    .sh-empty,
    .sh-panel {
        border: 1px solid var(--sh-border);
        border-radius: 18px;
        background: linear-gradient(180deg, rgba(18, 27, 38, 0.97), rgba(11, 18, 25, 0.99));
        padding: 0.82rem 0.95rem;
        margin-bottom: 0.88rem;
        box-shadow: 0 10px 24px rgba(0, 0, 0, 0.18);
    }

    .sh-weather-grid,
    .sh-config-grid,
    .sh-detail-grid,
    .sh-detail-top-grid,
    .sh-detail-bottom-grid {
        display: grid;
        gap: 0.8rem;
        grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
    }

    .sh-device-grid,
    .sh-room-grid {
        display: grid;
        gap: 0.76rem;
        grid-template-columns: repeat(auto-fit, minmax(265px, 1fr));
        align-items: stretch;
    }

    .sh-device-card {
        position: relative;
        overflow: hidden;
        display: flex;
        flex-direction: column;
        border: 1px solid var(--sh-border);
        border-radius: 20px;
        background:
            radial-gradient(circle at top right, rgba(123, 200, 255, 0.11), transparent 36%),
            linear-gradient(180deg, rgba(21, 32, 43, 0.98), rgba(11, 18, 24, 0.99));
        padding: 0.82rem;
        min-height: 16.9rem;
        box-shadow: 0 14px 28px rgba(0, 0, 0, 0.22);
    }

    .sh-room-card {
        border: 1px solid var(--sh-border);
        border-radius: 18px;
        background: var(--sh-surface-card);
        padding: 0.85rem;
        box-shadow: 0 1px 3px rgba(0, 0, 0, 0.28);
    }

    .sh-device-card::after {
        content: "";
        position: absolute;
        inset: auto -2rem -3rem auto;
        width: 8rem;
        height: 8rem;
        border-radius: 999px;
        background: radial-gradient(circle, rgba(123, 200, 255, 0.12), transparent 68%);
        pointer-events: none;
    }

    .sh-device-card.is-relay-card::after {
        background: radial-gradient(circle, rgba(255, 214, 107, 0.17), transparent 68%);
    }

    .sh-device-card.is-cover-card::after {
        background: radial-gradient(circle, rgba(125, 211, 252, 0.18), transparent 68%);
    }

    .sh-device-card.is-battery-card::after {
        background: radial-gradient(circle, rgba(134, 239, 172, 0.16), transparent 68%);
    }

    .sh-card-head,
    .sh-room-head {
        display: flex;
        justify-content: space-between;
        gap: 0.7rem;
        align-items: flex-start;
        margin-bottom: 0.55rem;
    }

    .sh-card-head h3,
    .sh-room-head h3 {
        margin: 0;
        font-size: 1rem;
    }

    .sh-card-topline {
        display: flex;
        justify-content: space-between;
        align-items: center;
        gap: 0.6rem;
        margin-bottom: 0.55rem;
    }

    .sh-device-kicker {
        display: inline-flex;
        align-items: center;
        gap: 0.38rem;
        font-size: 0.78rem;
        text-transform: uppercase;
        letter-spacing: 0.08em;
        color: #a8c4de;
    }

    .sh-identity {
        min-width: 0;
    }

    .sh-identity h3 {
        line-height: 1.18;
        margin-bottom: 0.2rem;
    }

    .sh-card-summary {
        margin: 0 0 0.68rem;
        color: #d6e4f2;
        font-size: 0.92rem;
    }

    .sh-card-badges {
        display: flex;
        gap: 0.35rem;
        flex-wrap: wrap;
        justify-content: flex-end;
    }

    .sh-primary-grid {
        display: grid;
        gap: 0.5rem;
        grid-template-columns: repeat(auto-fit, minmax(112px, 1fr));
        margin-bottom: 0.62rem;
    }

    .sh-primary-tile {
        border: 1px solid rgba(45, 65, 84, 0.8);
        border-radius: 14px;
        padding: 0.58rem 0.62rem;
        background: rgba(9, 16, 23, 0.45);
    }

    .sh-primary-tile span {
        display: block;
        font-size: 0.78rem;
        color: var(--sh-text-muted);
        margin-bottom: 0.16rem;
    }

    .sh-primary-tile strong {
        display: block;
        font-size: 1.02rem;
        line-height: 1.1;
    }

    .sh-value-list {
        display: grid;
        gap: 0.3rem;
        margin-bottom: 0.55rem;
    }

    .sh-value-row {
        display: flex;
        justify-content: space-between;
        gap: 0.6rem;
        border-bottom: 1px solid var(--sh-border-soft);
        padding-bottom: 0.22rem;
    }

    .sh-secondary-list,
    .sh-room-device-list,
    .sh-checkbox-list {
        display: flex;
        flex-wrap: wrap;
        gap: 0.42rem;
        margin-top: 0.45rem;
    }

    .sh-secondary-pill {
        border-radius: 999px;
        background: rgba(24, 36, 50, 0.88);
        padding: 0.3rem 0.52rem;
        font-size: 0.82rem;
        border: 1px solid rgba(45, 65, 84, 0.72);
        display: inline-flex;
        align-items: center;
        gap: 0.25rem;
    }

    .sh-control-section {
        margin-top: 0.78rem;
        padding-top: 0.72rem;
        border-top: 1px solid var(--sh-border-soft);
    }

    .sh-control-row,
    .sh-slider-row,
    .sh-form-row,
    .sh-chart-toolbar {
        display: flex;
        flex-wrap: wrap;
        gap: 0.45rem;
        align-items: center;
    }

    .sh-control-row {
        margin-bottom: 0.45rem;
    }

    .sh-section-head {
        display: flex;
        justify-content: space-between;
        align-items: center;
        gap: 0.65rem;
        margin-bottom: 0.55rem;
    }

    .sh-section-head h3 {
        margin: 0;
    }

    .sh-control-label {
        font-size: 0.8rem;
        text-transform: uppercase;
        letter-spacing: 0.08em;
        color: #9fc2de;
    }

    .sh-control-row .sh-btn.active {
        background: #403519;
        border-color: #a98d43;
        color: #ffe4a0;
    }

    .sh-lamp-grid {
        display: grid;
        gap: 0.58rem;
        grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
    }

    .sh-lamp-button {
        width: 100%;
        display: grid;
        justify-items: center;
        gap: 0.58rem;
        padding: 0.88rem 0.78rem 0.8rem;
        border-radius: 22px;
        border: 1px solid rgba(49, 66, 82, 0.9);
        background: linear-gradient(180deg, rgba(19, 28, 38, 0.96), rgba(9, 14, 20, 0.99));
        color: var(--sh-text);
        text-align: left;
        box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.04);
    }

    .sh-lamp-art {
        position: relative;
        width: 4.7rem;
        height: 5.35rem;
        display: grid;
        justify-items: center;
        align-content: start;
        padding-top: 0.18rem;
    }

    .sh-lamp-glow {
        position: absolute;
        inset: 0.1rem;
        border-radius: 999px;
        background: radial-gradient(circle, rgba(255, 214, 107, 0.82), rgba(255, 180, 33, 0.18) 46%, transparent 74%);
        opacity: 0;
        transform: scale(0.82);
        filter: blur(7px);
        transition: opacity 0.18s ease, transform 0.18s ease;
    }

    .sh-lamp-bulb {
        position: relative;
        width: 3.15rem;
        height: 3.7rem;
        border-radius: 1.72rem 1.72rem 1.18rem 1.18rem;
        border: 1px solid rgba(74, 91, 110, 0.88);
        background: linear-gradient(180deg, rgba(117, 129, 143, 0.24), rgba(29, 40, 52, 0.96));
        box-shadow:
            inset 0 1px 0 rgba(255, 255, 255, 0.05),
            inset 0 -0.55rem 0.85rem rgba(0, 0, 0, 0.26);
    }

    .sh-lamp-bulb::before {
        content: "";
        position: absolute;
        left: 0.58rem;
        top: 0.38rem;
        width: 0.82rem;
        height: 1.34rem;
        border-radius: 999px;
        background: linear-gradient(180deg, rgba(255, 255, 255, 0.24), transparent);
        transform: rotate(-18deg);
        opacity: 0.52;
    }

    .sh-lamp-filament {
        position: absolute;
        left: 50%;
        bottom: 0.72rem;
        width: 1.18rem;
        height: 0.72rem;
        transform: translateX(-50%);
        border: 2px solid rgba(166, 179, 193, 0.48);
        border-top: none;
        border-radius: 0 0 0.72rem 0.72rem;
    }

    .sh-lamp-cap {
        width: 1.78rem;
        height: 0.66rem;
        margin-top: -0.14rem;
        border-radius: 0 0 0.48rem 0.48rem;
        background: linear-gradient(180deg, #798696, #3c4855);
        box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.16);
    }

    .sh-lamp-base {
        width: 1.05rem;
        height: 0.46rem;
        margin-top: -0.08rem;
        border-radius: 0 0 0.28rem 0.28rem;
        background: #55626e;
    }

    .sh-lamp-copy {
        min-width: 0;
        display: grid;
        gap: 0.16rem;
        justify-items: center;
        text-align: center;
    }

    .sh-lamp-copy strong {
        font-size: 0.94rem;
        font-weight: 600;
    }

    .sh-lamp-copy small {
        color: var(--sh-text-muted);
        font-size: 0.77rem;
    }

    .sh-lamp-button.is-on {
        border-color: rgba(190, 153, 73, 0.82);
        background: linear-gradient(180deg, rgba(60, 44, 16, 0.94), rgba(27, 18, 6, 0.99));
        box-shadow:
            inset 0 1px 0 rgba(255, 246, 214, 0.08),
            0 10px 22px rgba(0, 0, 0, 0.2);
    }

    .sh-lamp-button.is-on .sh-lamp-glow {
        opacity: 1;
        transform: scale(1);
        animation: shPulseGlow 2.2s ease-in-out infinite;
    }

    .sh-lamp-button.is-on .sh-lamp-bulb {
        border-color: rgba(237, 197, 96, 0.86);
        background: radial-gradient(circle at 50% 28%, rgba(255, 248, 205, 0.98), rgba(255, 220, 136, 0.96) 42%, rgba(162, 105, 18, 0.95) 100%);
        box-shadow:
            0 0 0 1px rgba(255, 214, 107, 0.16),
            0 0 22px rgba(255, 194, 72, 0.24),
            inset 0 1px 0 rgba(255, 255, 255, 0.4);
    }

    .sh-lamp-button.is-on .sh-lamp-filament {
        border-color: rgba(123, 76, 5, 0.82);
    }

    .sh-lamp-button.is-on .sh-lamp-cap,
    .sh-lamp-button.is-on .sh-lamp-base {
        background: linear-gradient(180deg, #95846d, #5f4b2d);
    }

    .sh-lamp-state-row {
        display: grid;
        gap: 0.18rem;
        justify-items: center;
    }

    .sh-lamp-state-pill {
        min-width: 3.7rem;
        text-align: center;
        border-radius: 999px;
        border: 1px solid rgba(63, 81, 100, 0.9);
        background: rgba(15, 23, 32, 0.92);
        color: #9fb6cb;
        padding: 0.2rem 0.56rem;
        font-size: 0.72rem;
        font-weight: 700;
        letter-spacing: 0.08em;
        text-transform: uppercase;
    }

    .sh-lamp-button.is-on .sh-lamp-state-pill {
        border-color: rgba(232, 191, 93, 0.72);
        background: rgba(74, 54, 13, 0.92);
        color: #ffe59c;
    }

    .sh-cover-widget {
        display: grid;
        gap: 0.62rem;
    }

    .sh-cover-visual {
        display: grid;
        gap: 0.55rem;
        padding: 0.72rem;
        border-radius: 18px;
        border: 1px solid rgba(45, 65, 84, 0.84);
        background: linear-gradient(180deg, rgba(17, 28, 39, 0.96), rgba(8, 13, 19, 0.99));
    }

    .sh-cover-frame {
        display: flex;
        justify-content: center;
    }

    .sh-cover-window {
        position: relative;
        width: min(100%, 10.5rem);
        aspect-ratio: 0.92;
        border-radius: 16px 16px 12px 12px;
        padding: 0.46rem;
        background: linear-gradient(180deg, #dbe9f5, #8aa5be 55%, #5c7387);
        box-shadow: inset 0 0 0 1px rgba(255, 255, 255, 0.22);
        overflow: hidden;
    }

    .sh-cover-glass {
        position: absolute;
        inset: 0.44rem;
        border-radius: 12px 12px 8px 8px;
        background:
            linear-gradient(180deg, rgba(207, 230, 249, 0.94), rgba(138, 190, 228, 0.82)),
            linear-gradient(90deg, rgba(255, 255, 255, 0.18), transparent 50%);
    }

    .sh-cover-shutter {
        position: absolute;
        left: 0.44rem;
        right: 0.44rem;
        top: 0.44rem;
        min-height: 0;
        border-radius: 12px 12px 8px 8px;
        background:
            linear-gradient(180deg, rgba(238, 242, 246, 0.96), rgba(179, 191, 204, 0.98)),
            repeating-linear-gradient(180deg, rgba(118, 128, 139, 0.48) 0 5px, rgba(255, 255, 255, 0.08) 5px 9px);
        overflow: hidden;
        box-shadow: inset 0 -1px 0 rgba(46, 57, 70, 0.24);
    }

    .sh-cover-shutter--unknown {
        height: 58%;
        opacity: 0.86;
        background:
            linear-gradient(180deg, rgba(207, 212, 219, 0.88), rgba(128, 139, 149, 0.94)),
            repeating-linear-gradient(135deg, rgba(87, 99, 111, 0.34) 0 8px, rgba(229, 236, 242, 0.18) 8px 16px);
    }

    .sh-cover-shutter.is-moving-up::before,
    .sh-cover-shutter.is-moving-down::before {
        content: "";
        position: absolute;
        inset: 0;
        background: repeating-linear-gradient(180deg, rgba(255, 255, 255, 0.06) 0 5px, rgba(87, 99, 111, 0.18) 5px 10px);
    }

    .sh-cover-shutter.is-moving-up::before {
        animation: shShutterDriftUp 0.9s linear infinite;
    }

    .sh-cover-shutter.is-moving-down::before {
        animation: shShutterDriftDown 0.9s linear infinite;
    }

    .sh-cover-meta {
        display: flex;
        flex-wrap: wrap;
        gap: 0.42rem;
        justify-content: center;
    }

    .sh-cover-button-row {
        justify-content: center;
    }

    .sh-slider-row input[type="range"] {
        flex: 1 1 180px;
    }

    .sh-slider-row.is-disabled {
        opacity: 0.72;
    }

    .sh-card-footer,
    .sh-detail-footer {
        display: flex;
        gap: 0.55rem;
        justify-content: space-between;
        align-items: center;
        flex-wrap: wrap;
        margin-top: auto;
        padding-top: 0.78rem;
        border-top: 1px solid var(--sh-border-soft);
    }

    .sh-input,
    .sh-select,
    .sh-textarea {
        width: 100%;
        min-height: 2.2rem;
        border-radius: 10px;
        border: 1px solid var(--sh-border);
        padding: 0.5rem 0.7rem;
        background: var(--sh-surface-input);
        color: var(--sh-text);
        font: inherit;
    }

    .sh-textarea {
        min-height: 5.5rem;
        resize: vertical;
    }

    .sh-field {
        display: grid;
        gap: 0.28rem;
        min-width: 180px;
        flex: 1 1 220px;
    }

    .sh-field label {
        font-size: 0.84rem;
        color: var(--sh-text-muted);
    }

    .sh-form-shell {
        margin-top: 0.72rem;
        padding-top: 0.72rem;
        border-top: 1px solid var(--sh-border-soft);
    }

    .sh-danger-box {
        margin-top: 0.82rem;
        display: grid;
        gap: 0.6rem;
        border: 1px solid rgba(122, 62, 62, 0.72);
        border-radius: 16px;
        padding: 0.8rem;
        background: linear-gradient(180deg, rgba(37, 18, 20, 0.96), rgba(20, 10, 12, 0.99));
    }

    .sh-danger-box strong {
        display: block;
        margin-bottom: 0.15rem;
    }

    .sh-technical-table,
    .sh-log-table {
        width: 100%;
        border-collapse: collapse;
        margin-top: 0.65rem;
        font-size: 0.88rem;
    }

    .sh-technical-table th,
    .sh-technical-table td,
    .sh-log-table th,
    .sh-log-table td {
        border-bottom: 1px solid var(--sh-border-soft);
        padding: 0.4rem 0.35rem;
        vertical-align: top;
        text-align: left;
    }

    .sh-chart-shell {
        margin-top: 0.8rem;
        border: 1px solid var(--sh-border);
        border-radius: 16px;
        background: #0f161f;
        padding: 0.75rem;
    }

    .sh-chart-stack {
        display: grid;
        gap: 0.72rem;
    }

    .sh-chart-panel {
        border: 1px solid var(--sh-border-soft);
        border-radius: 14px;
        background: rgba(8, 12, 17, 0.74);
        padding: 0.7rem;
    }

    .sh-chart-panel-head {
        display: flex;
        justify-content: space-between;
        align-items: center;
        gap: 0.55rem;
        flex-wrap: wrap;
        margin-bottom: 0.55rem;
    }

    .sh-chart-panel-head strong {
        display: block;
        font-size: 0.92rem;
    }

    .sh-chart-panel-copy {
        display: grid;
        gap: 0.15rem;
    }

    .sh-chart-panel-stats {
        display: flex;
        gap: 0.38rem;
        flex-wrap: wrap;
    }

    .sh-chart-meta {
        display: flex;
        gap: 0.45rem;
        flex-wrap: wrap;
        margin-bottom: 0.6rem;
    }

    .sh-chart-axis-row {
        display: flex;
        justify-content: space-between;
        gap: 0.55rem;
        margin-top: 0.45rem;
        font-size: 0.78rem;
        color: var(--sh-text-muted);
    }

    .sh-chart-legend {
        display: flex;
        gap: 0.65rem;
        flex-wrap: wrap;
        margin-top: 0.55rem;
    }

    .sh-chart-legend span {
        display: grid;
        gap: 0.08rem;
        min-width: 160px;
        padding: 0.1rem 0;
        font-size: 0.84rem;
    }

    .sh-chart-legend strong {
        display: inline-flex;
        align-items: center;
        gap: 0.35rem;
        font-size: 0.84rem;
        font-weight: 600;
    }

    .sh-chart-legend small {
        color: var(--sh-text-muted);
        font-size: 0.78rem;
    }

    .sh-chart-legend i {
        width: 1rem;
        height: 0.32rem;
        border-radius: 999px;
        display: inline-block;
    }

    .sh-chart-svg {
        width: 100%;
        overflow: visible;
    }

    .sh-chart-note,
    .sh-chart-state {
        display: grid;
        gap: 0.18rem;
        margin-top: 0.6rem;
        border-radius: 12px;
        border: 1px solid var(--sh-border-soft);
        padding: 0.7rem 0.75rem;
        background: rgba(14, 21, 29, 0.92);
    }

    .sh-chart-note {
        margin-bottom: 0.65rem;
    }

    .sh-chart-note strong,
    .sh-chart-state strong {
        font-size: 0.86rem;
    }

    .sh-chart-note p,
    .sh-chart-state p {
        margin: 0;
        color: var(--sh-text-muted);
        font-size: 0.84rem;
    }

    .sh-chart-state.is-invalid {
        border-color: rgba(176, 142, 68, 0.84);
        background: linear-gradient(180deg, rgba(45, 35, 14, 0.94), rgba(27, 20, 7, 0.98));
    }

    .sh-chart-state.is-no-data {
        border-color: rgba(57, 77, 97, 0.88);
    }

    .sh-chart-state.is-error {
        border-color: rgba(143, 70, 70, 0.9);
        background: linear-gradient(180deg, rgba(44, 20, 21, 0.95), rgba(26, 12, 13, 0.99));
    }

    .sh-chart-state.is-warning,
    .sh-chart-note.is-warning {
        border-color: rgba(93, 115, 57, 0.88);
        background: linear-gradient(180deg, rgba(29, 37, 18, 0.95), rgba(18, 23, 11, 0.99));
    }

    .sh-code-block {
        white-space: pre-wrap;
        word-break: break-word;
        margin: 0;
        font-family: "Courier New", Courier, monospace;
        font-size: 0.82rem;
        line-height: 1.35;
    }

    .sh-divider {
        margin: 0.85rem 0;
        border: none;
        border-top: 1px solid var(--sh-border-soft);
    }

    @keyframes shPulseGlow {
        0%,
        100% {
            box-shadow:
                0 0 0 1px rgba(255, 214, 107, 0.18),
                0 0 14px rgba(255, 193, 7, 0.18);
        }
        50% {
            box-shadow:
                0 0 0 1px rgba(255, 214, 107, 0.28),
                0 0 24px rgba(255, 193, 7, 0.28);
        }
    }

    @keyframes shShutterDriftUp {
        from { transform: translateY(0); }
        to { transform: translateY(-10px); }
    }

    @keyframes shShutterDriftDown {
        from { transform: translateY(0); }
        to { transform: translateY(10px); }
    }

    @media (min-width: 960px) {
        .sh-detail-top-grid {
            grid-template-columns: minmax(0, 1.25fr) minmax(320px, 0.95fr);
        }

        .sh-detail-bottom-grid {
            grid-template-columns: minmax(0, 1fr) minmax(0, 1fr);
        }
    }

    @media (max-width: 640px) {
        .sh-toolbar-actions {
            justify-content: flex-start;
        }

        .sh-card-head,
        .sh-room-head,
        .sh-card-topline,
        .sh-section-head,
        .sh-card-footer {
            flex-direction: column;
            align-items: flex-start;
        }

        .sh-device-grid {
            grid-template-columns: 1fr;
        }

        .sh-lamp-grid {
            grid-template-columns: 1fr;
        }
    }
</style>
`;

const buildDeviceGridTemplate = ({ viewKey, title, intro, emptyText }) => `
<template>
    <div class="sh-page">
        <div class="sh-toolbar">
            <div class="sh-toolbar-actions">
                <button class="sh-btn sh-btn-muted" @click="refresh">Neu laden</button>
                ${renderDashboardNavHtml()}
            </div>
        </div>

        <div class="sh-status-row" v-if="payload.page && payload.page.key === 'overview' && payload.summary">
            <span class="sh-chip">{{ payload.summary.device_count || 0 }} Geräte</span>
            <span class="sh-chip">{{ payload.summary.master_count || 0 }} Master</span>
        </div>

        <div class="sh-empty" v-if="!deviceList.length">${emptyText}</div>

        <div class="sh-device-grid" v-else>
            <article class="sh-device-card" :class="device.surface_class" v-for="device in deviceList" :key="device.device_id">
                <div class="sh-card-topline">
                    <span class="sh-device-kicker">
                        <i :class="'mdi ' + device.type_icon"></i>
                        {{ device.kind_label }}
                    </span>
                    <div class="sh-card-badges">
                        <span class="sh-chip sh-chip-power">
                            <i :class="'mdi ' + device.power_icon"></i>
                            {{ device.power_label }}
                        </span>
                        <span class="sh-chip sh-chip-sim" v-if="device.simulation">SIM</span>
                        <span class="sh-chip" :class="device.online_class">{{ device.online_label }}</span>
                    </div>
                </div>

                <div class="sh-card-head">
                    <div class="sh-identity">
                        <h3>{{ device.display_name }}</h3>
                        <p class="sh-muted">{{ device.device_id }}<span v-if="device.room_name"> · {{ device.room_name }}</span></p>
                    </div>
                </div>

                <p class="sh-card-summary" v-if="device.summary_label">{{ device.summary_label }}</p>

                <div class="sh-primary-grid" v-if="device.highlight_values && device.highlight_values.length">
                    <div class="sh-primary-tile" v-for="item in device.highlight_values" :key="device.device_id + '-' + item.key">
                        <span>{{ item.label }}</span>
                        <strong>{{ item.value_text }}</strong>
                    </div>
                </div>

                <div class="sh-secondary-list" v-if="device.secondary_values && device.secondary_values.length">
                    <span class="sh-secondary-pill" v-for="item in device.secondary_values" :key="device.device_id + '-extra-' + item.key">
                        {{ item.label }}: {{ item.value_text }}
                    </span>
                </div>

                <div class="sh-control-section" v-if="payload.page && payload.page.show_controls !== false && device.controls && device.controls.kind === 'relay'">
                    <div class="sh-section-head">
                        <span class="sh-control-label">Schaltkanäle</span>
                    </div>
                    <div class="sh-lamp-grid">
                        <button
                            class="sh-lamp-button"
                            v-for="relay in device.controls.relays"
                            :key="device.device_id + '-' + relay.key"
                            :class="{ 'is-on': relay.active }"
                            type="button"
                            :aria-pressed="relay.active ? 'true' : 'false'"
                            @click="relaySet(device.device_id, relay.key, !relay.active)">
                            <span class="sh-lamp-art" aria-hidden="true">
                                <span class="sh-lamp-glow"></span>
                                <span class="sh-lamp-bulb">
                                    <span class="sh-lamp-filament"></span>
                                </span>
                                <span class="sh-lamp-cap"></span>
                                <span class="sh-lamp-base"></span>
                            </span>
                            <span class="sh-lamp-copy">
                                <strong>{{ relay.label }}</strong>
                                <small>{{ relay.action_label }}</small>
                            </span>
                            <span class="sh-lamp-state-row">
                                <span class="sh-lamp-state-pill">{{ relay.state_label }}</span>
                            </span>
                        </button>
                    </div>
                </div>

                <div class="sh-control-section" v-if="payload.page && payload.page.show_controls !== false && device.controls && device.controls.kind === 'cover'">
                    <div class="sh-section-head">
                        <span class="sh-control-label">Rollladen</span>
                        <span class="sh-muted">{{ device.controls.calibration_label }}</span>
                    </div>
                    <div class="sh-cover-widget">
                        <div class="sh-cover-visual">
                            <div class="sh-cover-frame">
                                <div class="sh-cover-window">
                                    <div class="sh-cover-glass"></div>
                                    <div
                                        class="sh-cover-shutter"
                                        v-if="device.controls.position_known"
                                        :class="device.controls.motion_class"
                                        :style="device.controls.shutter_style"></div>
                                    <div
                                        class="sh-cover-shutter sh-cover-shutter--unknown"
                                        v-else
                                        :class="device.controls.motion_class"></div>
                                </div>
                            </div>
                            <div class="sh-cover-meta">
                                <span class="sh-secondary-pill">Position: {{ device.controls.position_text }}</span>
                                <span class="sh-secondary-pill">
                                    <i :class="'mdi ' + device.controls.state_icon"></i>
                                    {{ device.controls.state_text }}
                                </span>
                            </div>
                        </div>
                        <div class="sh-control-row sh-cover-button-row">
                            <button class="sh-btn" @click="coverMove(device.device_id, 'up')"><i class="mdi mdi-arrow-up"></i> Auf</button>
                            <button class="sh-btn" @click="coverMove(device.device_id, 'stop')"><i class="mdi mdi-stop"></i> Stopp</button>
                            <button class="sh-btn" @click="coverMove(device.device_id, 'down')"><i class="mdi mdi-arrow-down"></i> Ab</button>
                        </div>
                        <div class="sh-slider-row" :class="{ 'is-disabled': !device.controls.slider_enabled }">
                            <input type="range" min="0" max="100" :value="device.controls.position_value" :disabled="!device.controls.slider_enabled" />
                            <strong>{{ device.controls.position_text }}</strong>
                        </div>
                        <p class="sh-muted">{{ device.controls.slider_note }}</p>
                    </div>
                </div>

                <div class="sh-card-footer">
                    <span class="sh-muted" :title="device.last_seen_exact_label">Zuletzt gesehen: {{ device.last_seen_label }}</span>
                    <a class="sh-btn" :href="device.detail_url">Details</a>
                </div>
            </article>
        </div>
    </div>
</template>

<script>
export default {
    computed: {
        payload() {
            return this.msg?.payload || { page: {}, devices: [], summary: {} };
        },
        deviceList() {
            return Array.isArray(this.payload.devices) ? this.payload.devices : [];
        }
    },
    methods: {
        sendAction(action, extra = {}) {
            this.send({ payload: Object.assign({ view: "${viewKey}", action }, extra) });
        },
        refresh() {
            this.sendAction("load");
        },
        relaySet(deviceId, relayKey, state) {
            this.sendAction("relay_set", { device_id: deviceId, relay_key: relayKey, state });
        },
        coverMove(deviceId, direction) {
            this.sendAction("cover_move", { device_id: deviceId, direction });
        }
    },
    mounted() {
        setTimeout(() => this.refresh(), 120);
    }
}
</script>

${dashboardSharedStyle}
`;

const roomsTemplate = `
<template>
    <div class="sh-page">
        <div class="sh-toolbar">
            <div class="sh-toolbar-actions">
                <button class="sh-btn sh-btn-muted" @click="refresh">Neu laden</button>
                ${renderDashboardNavHtml()}
            </div>
        </div>

        <div class="sh-empty" v-if="!roomList.length">Noch keine Räume sichtbar. Räume entstehen über das Gerätedetail oder aus vorhandenen Zuordnungen.</div>

        <div class="sh-room-grid" v-else>
            <article class="sh-room-card" v-for="room in roomList" :key="room.slug">
                <div class="sh-room-head">
                    <div>
                        <h3>{{ room.name }}</h3>
                        <p class="sh-muted">{{ room.device_count }} Geräte</p>
                    </div>
                    <div class="sh-card-badges">
                        <span class="sh-chip sh-chip-online">{{ room.online_count }} online</span>
                        <span class="sh-chip sh-chip-offline" v-if="room.offline_count">{{ room.offline_count }} offline</span>
                    </div>
                </div>
                <div class="sh-room-device-list">
                    <span class="sh-secondary-pill" v-for="device in room.devices" :key="room.slug + '-' + device.device_id">
                        <a :href="device.detail_url">{{ device.display_name }}</a>
                    </span>
                </div>
            </article>
        </div>
    </div>
</template>

<script>
export default {
    computed: {
        roomList() {
            return Array.isArray(this.msg?.payload?.rooms) ? this.msg.payload.rooms : [];
        }
    },
    methods: {
        refresh() {
            this.send({ payload: { view: "rooms", action: "load" } });
        }
    },
    mounted() {
        setTimeout(() => this.refresh(), 120);
    }
}
</script>

${dashboardSharedStyle}
`;

const deviceDetailTemplate = `
<template>
    <div class="sh-page">
        <div class="sh-toolbar">
            <div class="sh-toolbar-actions">
                <button class="sh-btn sh-btn-muted" @click="reload">Neu laden</button>
                <a class="sh-btn sh-btn-muted" href="/dashboard/geraete">Zur Geräteübersicht</a>
            </div>
        </div>

        <div class="sh-empty" v-if="removedMessage">
            {{ removedMessage }}
        </div>

        <div v-else-if="!detail.device" class="sh-empty">
            Kein Gerät ausgewählt. Öffne die Seite über einen Detail-Link aus der Übersicht oder über
            <a href="/dashboard/geraete">/dashboard/geraete</a>.
        </div>

        <div v-else>
            <div class="sh-detail-top-grid">
                <section class="sh-panel" :class="detail.device.surface_class">
                    <div class="sh-card-topline">
                        <span class="sh-device-kicker">
                            <i :class="'mdi ' + detail.device.type_icon"></i>
                            {{ detail.device.kind_label }}
                        </span>
                        <div class="sh-card-badges">
                            <span class="sh-chip sh-chip-power">
                                <i :class="'mdi ' + detail.device.power_icon"></i>
                                {{ detail.device.power_label }}
                            </span>
                            <span class="sh-chip" :class="detail.device.online_class">{{ detail.device.online_label }}</span>
                        </div>
                    </div>
                    <div class="sh-card-head">
                        <div class="sh-identity">
                            <h3>{{ detail.device.display_name }}</h3>
                            <p class="sh-muted">{{ detail.device.device_id }}</p>
                        </div>
                    </div>
                    <p class="sh-card-summary" v-if="detail.device.summary_label">{{ detail.device.summary_label }}</p>
                    <div class="sh-primary-grid" v-if="detail.device.primary_values && detail.device.primary_values.length">
                        <div class="sh-primary-tile" v-for="item in detail.device.primary_values" :key="'detail-value-' + item.key">
                            <span>{{ item.label }}</span>
                            <strong>{{ item.value_text }}</strong>
                        </div>
                    </div>
                    <div class="sh-form-shell">
                        <div class="sh-section-head">
                            <span class="sh-control-label">Gerätezuordnung</span>
                            <span class="sh-muted">{{ detail.device.relay_name_fields && detail.device.relay_name_fields.length ? 'Schreibbar: Anzeigename, Raum und Dashboard-Relaisnamen' : 'Schreibbar: Anzeigename und Raum' }}</span>
                        </div>
                        <div class="sh-form-row">
                            <div class="sh-field">
                                <label for="detail-display-name">Anzeigename</label>
                                <input id="detail-display-name" class="sh-input" v-model="form.display_name" type="text" />
                            </div>
                            <div class="sh-field">
                                <label for="detail-room-name">Raum</label>
                                <input id="detail-room-name" class="sh-input" v-model="form.room_name" type="text" list="detail-room-options" />
                                <datalist id="detail-room-options">
                                    <option v-for="room in detail.room_options" :key="room.slug" :value="room.name"></option>
                                </datalist>
                            </div>
                        </div>
                        <div class="sh-form-row" v-if="detail.device.relay_name_fields && detail.device.relay_name_fields.length">
                            <div class="sh-field" v-for="field in detail.device.relay_name_fields" :key="'relay-name-' + field.key">
                                <label :for="'detail-relay-name-' + field.key">{{ field.label }}</label>
                                <input
                                    :id="'detail-relay-name-' + field.key"
                                    class="sh-input"
                                    v-model="form.relay_names[field.key]"
                                    type="text"
                                    :placeholder="field.placeholder" />
                            </div>
                        </div>
                        <p class="sh-muted" v-if="detail.device.relay_name_fields && detail.device.relay_name_fields.length">
                            Relaisnamen wirken nur im Dashboard und werden als UI-Konfiguration in SQLite gespeichert.
                        </p>
                        <div class="sh-control-row">
                            <button class="sh-btn" @click="saveMeta">{{ detail.device.relay_name_fields && detail.device.relay_name_fields.length ? 'Anzeige / Raum / Relais speichern' : 'Anzeige / Raum speichern' }}</button>
                        </div>
                    </div>
                    <hr class="sh-divider" />
                    <div class="sh-value-list">
                        <div class="sh-value-row" v-for="item in detail.device.info_rows" :key="item.key">
                            <span>{{ item.label }}</span>
                            <strong>{{ item.value_text }}</strong>
                        </div>
                    </div>
                </section>

                <section class="sh-panel">
                    <div class="sh-section-head">
                        <div>
                            <h3>Steuerung</h3>
                            <p class="sh-muted">Direkt auf den offiziellen cmd/set-Pfad verdrahtet.</p>
                        </div>
                    </div>
                    <div v-if="detail.device.controls">
                        <div class="sh-control-section" v-if="detail.device.controls.kind === 'relay'">
                            <div class="sh-section-head">
                                <span class="sh-control-label">Schaltkanäle</span>
                            </div>
                            <div class="sh-lamp-grid">
                                <button
                                    class="sh-lamp-button"
                                    v-for="relay in detail.device.controls.relays"
                                    :key="relay.key"
                                    :class="{ 'is-on': relay.active }"
                                    type="button"
                                    :aria-pressed="relay.active ? 'true' : 'false'"
                                    @click="relaySet(relay.key, !relay.active)">
                                    <span class="sh-lamp-art" aria-hidden="true">
                                        <span class="sh-lamp-glow"></span>
                                        <span class="sh-lamp-bulb">
                                            <span class="sh-lamp-filament"></span>
                                        </span>
                                        <span class="sh-lamp-cap"></span>
                                        <span class="sh-lamp-base"></span>
                                    </span>
                                    <span class="sh-lamp-copy">
                                        <strong>{{ relay.label }}</strong>
                                        <small>{{ relay.action_label }}</small>
                                    </span>
                                    <span class="sh-lamp-state-row">
                                        <span class="sh-lamp-state-pill">{{ relay.state_label }}</span>
                                    </span>
                                </button>
                            </div>
                        </div>
                        <div class="sh-control-section" v-else-if="detail.device.controls.kind === 'cover'">
                            <div class="sh-section-head">
                                <span class="sh-control-label">Rollladen</span>
                                <span class="sh-muted">{{ detail.device.controls.calibration_label }}</span>
                            </div>
                            <div class="sh-cover-widget">
                                <div class="sh-cover-visual">
                                    <div class="sh-cover-frame">
                                        <div class="sh-cover-window">
                                            <div class="sh-cover-glass"></div>
                                            <div
                                                class="sh-cover-shutter"
                                                v-if="detail.device.controls.position_known"
                                                :class="detail.device.controls.motion_class"
                                                :style="detail.device.controls.shutter_style"></div>
                                            <div
                                                class="sh-cover-shutter sh-cover-shutter--unknown"
                                                v-else
                                                :class="detail.device.controls.motion_class"></div>
                                        </div>
                                    </div>
                                    <div class="sh-cover-meta">
                                        <span class="sh-secondary-pill">Position: {{ detail.device.controls.position_text }}</span>
                                        <span class="sh-secondary-pill">
                                            <i :class="'mdi ' + detail.device.controls.state_icon"></i>
                                            {{ detail.device.controls.state_text }}
                                        </span>
                                    </div>
                                </div>
                                <div class="sh-control-row sh-cover-button-row">
                                    <button class="sh-btn" @click="coverMove('up')"><i class="mdi mdi-arrow-up"></i> Auf</button>
                                    <button class="sh-btn" @click="coverMove('stop')"><i class="mdi mdi-stop"></i> Stopp</button>
                                    <button class="sh-btn" @click="coverMove('down')"><i class="mdi mdi-arrow-down"></i> Ab</button>
                                </div>
                                <div class="sh-slider-row" :class="{ 'is-disabled': !detail.device.controls.slider_enabled }">
                                    <input type="range" min="0" max="100" :value="detail.device.controls.position_value" :disabled="!detail.device.controls.slider_enabled" />
                                    <strong>{{ detail.device.controls.position_text }}</strong>
                                </div>
                                <p class="sh-muted">{{ detail.device.controls.slider_note }}</p>
                            </div>
                        </div>
                    </div>
                    <div class="sh-empty" v-else>Für dieses Gerät ist aktuell keine Direktbedienung vorhanden.</div>
                    <div class="sh-danger-box" v-if="detail.device.delete_supported">
                        <div>
                            <strong>Gerät aus SQLite entfernen</strong>
                            <p class="sh-muted">Löscht nur Registry- und UI-Daten. Neue MQTT-Nachrichten können das Gerät später erneut anlegen.</p>
                        </div>
                        <button class="sh-btn sh-btn-danger" @click="removeDevice">Gerät entfernen</button>
                    </div>
                </section>
            </div>

            <section class="sh-panel">
                <div class="sh-section-head">
                    <div>
                        <h3>Diagramme</h3>
                        <p class="sh-muted">Nur für numerische Sensorzeitreihen mit Influx-Datenbasis.</p>
                    </div>
                </div>
                <div class="sh-chart-toolbar" v-if="detail.chart_metrics && detail.chart_metrics.length">
                    <div class="sh-field">
                        <label>Zeitraum</label>
                        <select class="sh-select" v-model="chartForm.range">
                            <option value="1h">1h</option>
                            <option value="6h">6h</option>
                            <option value="24h">24h</option>
                            <option value="7d">7d</option>
                            <option value="30d">30d</option>
                        </select>
                    </div>
                    <div class="sh-field">
                        <label>Schrittweite</label>
                        <select class="sh-select" v-model="chartForm.step">
                            <option value="auto">Auto</option>
                            <option value="raw">Rohdaten</option>
                            <option value="10s">10s</option>
                            <option value="30s">30s</option>
                            <option value="1m">1m</option>
                            <option value="5m">5m</option>
                            <option value="15m">15m</option>
                            <option value="1h">1h</option>
                        </select>
                    </div>
                </div>
                <div class="sh-checkbox-list" v-if="detail.chart_metrics && detail.chart_metrics.length">
                    <label class="sh-secondary-pill" v-for="metric in detail.chart_metrics" :key="metric.key">
                        <input type="checkbox" :value="metric.key" v-model="chartForm.metrics" />
                        {{ metric.label }}
                    </label>
                </div>
                <div class="sh-control-row" v-if="detail.chart_metrics && detail.chart_metrics.length">
                    <button class="sh-btn" @click="loadChart">Diagramm laden</button>
                </div>
                <div class="sh-empty" v-else>{{ detail.chart_unavailable_reason || "Keine chartbaren Sensorzeitreihen für dieses Gerät verfügbar." }}</div>

                <template v-if="chart.panels && chart.panels.length">
                    <div class="sh-chart-shell">
                        <div class="sh-chart-meta">
                            <span class="sh-chip">Gerät {{ chart.title }}</span>
                            <span class="sh-chip">Zeitraum {{ chart.range_label }}</span>
                            <span class="sh-chip">Raster {{ chart.step_label }}</span>
                            <span class="sh-chip" v-if="chart.metric_summary">Kurven {{ chart.metric_summary }}</span>
                        </div>
                        <div class="sh-chart-note" v-if="chart.notice" :class="chart.status === 'sparse' ? 'is-warning' : ''">
                            <strong>Hinweis</strong>
                            <p>{{ chart.notice }}</p>
                        </div>
                        <div class="sh-chart-stack">
                            <section class="sh-chart-panel" v-for="panel in chart.panels" :key="'detail-panel-' + panel.unit_key">
                                <div class="sh-chart-panel-head">
                                    <div class="sh-chart-panel-copy">
                                        <strong>{{ panel.axis_label }}</strong>
                                        <span class="sh-muted">{{ panel.series.length }} Kurve(n)<template v-if="panel.series.length > 1"> · {{ panel.metric_summary }}</template></span>
                                    </div>
                                    <div class="sh-chart-panel-stats">
                                        <span class="sh-chip">min {{ panel.min_label }}</span>
                                        <span class="sh-chip">max {{ panel.max_label }}</span>
                                        <span class="sh-chip">{{ panel.point_count }} Punkte</span>
                                    </div>
                                </div>
                                <svg class="sh-chart-svg" viewBox="0 0 640 280" width="100%" height="280" role="img" :aria-label="'Sensorverlauf ' + panel.axis_label">
                                    <g v-for="tick in panel.y_ticks" :key="'detail-y-' + panel.unit_key + '-' + tick.value_num">
                                        <line x1="72" :y1="tick.y" x2="618" :y2="tick.y" stroke="rgba(185, 199, 213, 0.16)" stroke-dasharray="4 4" />
                                        <text x="64" :y="tick.y + 4" fill="#8fa5ba" font-size="12" text-anchor="end">{{ tick.label }}</text>
                                    </g>
                                    <g v-for="tick in chart.x_ticks" :key="'detail-x-' + panel.unit_key + '-' + tick.ts_ms">
                                        <line :x1="tick.x" y1="28" :x2="tick.x" y2="212" stroke="rgba(185, 199, 213, 0.1)" stroke-dasharray="4 4" />
                                        <text :x="tick.x" y="234" fill="#8fa5ba" font-size="12" text-anchor="middle">{{ tick.label }}</text>
                                    </g>
                                    <line x1="72" y1="28" x2="72" y2="212" stroke="#b9c7d5" stroke-width="1" />
                                    <line x1="72" y1="212" x2="618" y2="212" stroke="#b9c7d5" stroke-width="1" />
                                    <text x="22" y="120" fill="#9fc2de" font-size="12" text-anchor="middle" transform="rotate(-90 22 120)">{{ panel.axis_label }}</text>
                                    <text x="345" y="258" fill="#9fc2de" font-size="12" text-anchor="middle">{{ chart.x_axis_label }}</text>
                                    <path
                                        v-for="series in panel.series"
                                        :key="'detail-series-' + panel.unit_key + '-' + series.metric"
                                        :d="seriesPath(series)"
                                        fill="none"
                                        :stroke="series.color"
                                        :stroke-width="seriesStrokeWidth(series)"
                                        stroke-linecap="round"
                                        stroke-linejoin="round" />
                                    <g v-for="series in panel.series" :key="'detail-points-' + panel.unit_key + '-' + series.metric">
                                        <circle
                                            v-for="point in series.points"
                                            :key="'detail-point-' + series.metric + '-' + point.ts_ms + '-' + point.value_num"
                                            :cx="point.x"
                                            :cy="point.y"
                                            :r="pointRadius(series)"
                                            :fill="series.color"
                                            stroke="#0f161f"
                                            stroke-width="1.4">
                                            <title>{{ pointTitle(series, point) }}</title>
                                        </circle>
                                    </g>
                                </svg>
                                <div class="sh-chart-axis-row">
                                    <span>{{ chart.x_axis_label }}</span>
                                    <span>{{ chart.start_label }} bis {{ chart.end_label }}</span>
                                </div>
                                <div class="sh-chart-legend">
                                    <span v-for="series in panel.series" :key="'detail-legend-' + panel.unit_key + '-' + series.metric">
                                        <strong><i :style="{ background: series.color }"></i>{{ series.label }}</strong>
                                        <small>Aktuell {{ series.last_value_text }} · {{ series.point_count }} Punkte<template v-if="!series.has_drawable_line"> · Einzelpunkt</template></small>
                                    </span>
                                </div>
                            </section>
                        </div>
                    </div>
                </template>
                <template v-else-if="chart.message">
                    <div class="sh-chart-meta" v-if="chart.title || chart.range_label || chart.step_label || chart.metric_summary">
                        <span class="sh-chip" v-if="chart.title">Gerät {{ chart.title }}</span>
                        <span class="sh-chip" v-if="chart.range_label">Zeitraum {{ chart.range_label }}</span>
                        <span class="sh-chip" v-if="chart.step_label">Raster {{ chart.step_label }}</span>
                        <span class="sh-chip" v-if="chart.metric_summary">Kurven {{ chart.metric_summary }}</span>
                    </div>
                    <div class="sh-chart-state" :class="chartStateClass(chart.status)">
                        <strong>{{ chartStateTitle(chart.status) }}</strong>
                        <p>{{ chart.message }}</p>
                    </div>
                </template>
            </section>

            <div class="sh-detail-bottom-grid">
                <section class="sh-panel">
                    <div class="sh-section-head">
                        <div>
                            <h3>Technische Details</h3>
                            <p class="sh-muted">Read-only Sicht auf bekannte Metadaten und Zeitstempel.</p>
                        </div>
                    </div>
                    <table class="sh-technical-table">
                        <thead>
                            <tr>
                                <th>Feld</th>
                                <th>Wert</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr v-for="item in detail.technical_rows" :key="'tech-' + item.key">
                                <td>{{ item.label }}</td>
                                <td>{{ item.value_text }}</td>
                            </tr>
                        </tbody>
                    </table>
                </section>

                <section class="sh-panel">
                    <div class="sh-section-head">
                        <div>
                            <h3>Gemeldete Gerätekonfiguration</h3>
                            <p class="sh-muted">Aus cfg/report, aktuell read-only.</p>
                        </div>
                    </div>
                    <table class="sh-technical-table" v-if="detail.config_rows && detail.config_rows.length">
                        <thead>
                            <tr>
                                <th>Key</th>
                                <th>Wert</th>
                                <th>Quelle</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr v-for="item in detail.config_rows" :key="'cfg-' + item.key">
                                <td>{{ item.key }}</td>
                                <td>{{ item.value_text }}</td>
                                <td>{{ item.source }}</td>
                            </tr>
                        </tbody>
                    </table>
                    <div class="sh-empty" v-else>Keine cfg/report-Daten vorhanden.</div>
                </section>
            </div>
        </div>
    </div>
</template>

<script>
export default {
    data() {
        return {
            detail: { room_options: [], sensor_metrics: [], chart_metrics: [], chart_unavailable_reason: "" },
            chart: {},
            form: { display_name: "", room_name: "", relay_names: {} },
            chartForm: { range: "24h", step: "auto", metrics: [] },
            removedMessage: "",
            autoChartLoaded: false
        };
    },
    methods: {
        currentDeviceId() {
            const params = new URLSearchParams(window.location.search || "");
            return params.get("device") || "";
        },
        sendAction(action, extra = {}) {
            const deviceId = extra.device_id || this.currentDeviceId();
            this.send({ payload: Object.assign({ view: "detail", action, device_id: deviceId }, extra) });
        },
        reload() {
            this.removedMessage = "";
            this.autoChartLoaded = false;
            this.chart = {};
            this.sendAction("load");
        },
        saveMeta() {
            this.sendAction("save", {
                display_name: this.form.display_name,
                room_name: this.form.room_name,
                relay_names: this.form.relay_names || {}
            });
        },
        removeDevice() {
            this.sendAction("delete");
        },
        relaySet(relayKey, state) {
            this.sendAction("relay_set", { relay_key: relayKey, state });
        },
        coverMove(direction) {
            this.sendAction("cover_move", { direction });
        },
        loadChart() {
            this.sendAction("chart", {
                metrics: this.chartForm.metrics,
                range: this.chartForm.range,
                step: this.chartForm.step,
                device_label: this.detail.device ? (this.detail.device.display_name || this.detail.device.device_id) : this.currentDeviceId(),
                chart_target: "detail"
            });
        },
        seriesPath(series) {
            if (!series || !Array.isArray(series.points) || !series.points.length) {
                return "";
            }
            return series.points
                .map((point, index) => (index === 0 ? "M" : "L") + point.x + " " + point.y)
                .join(" ");
        },
        seriesStrokeWidth(series) {
            if (!series || !series.point_count) return 2.2;
            return series.point_count <= 8 ? 2.8 : series.point_count <= 24 ? 2.4 : 2.2;
        },
        pointRadius(series) {
            if (!series || !series.point_count) return 3.2;
            if (!series.has_drawable_line) return 4.4;
            return series.point_count <= 8 ? 3.6 : series.point_count <= 24 ? 3.1 : 2.7;
        },
        chartStateClass(status) {
            return {
                "is-invalid": status === "invalid_selection",
                "is-no-data": status === "no_data",
                "is-error": status === "query_error" || status === "render_error",
                "is-warning": status === "sparse"
            };
        },
        chartStateTitle(status) {
            if (status === "invalid_selection") return "Ungültige Auswahl";
            if (status === "no_data") return "Keine Influx-Daten";
            if (status === "query_error") return "Influx-Fehler";
            if (status === "render_error") return "Darstellungsproblem";
            if (status === "sparse") return "Wenige Datenpunkte";
            return "Diagrammstatus";
        },
        pointTitle(series, point) {
            return [series && series.label, this.formatTimestamp(point && point.ts_ms), point && point.value_text].filter(Boolean).join(" · ");
        },
        formatTimestamp(tsMs) {
            if (!Number.isFinite(tsMs)) return "";
            const date = new Date(tsMs);
            if (Number.isNaN(date.getTime())) return "";
            return date.toLocaleString("de-DE", {
                day: "2-digit",
                month: "2-digit",
                hour: "2-digit",
                minute: "2-digit",
                second: "2-digit"
            });
        },
        applyDetailPayload(payload) {
            const currentDeviceId = this.currentDeviceId();
            if (payload.device && currentDeviceId && payload.device.device_id && payload.device.device_id !== currentDeviceId) {
                return;
            }
            this.detail = payload;
            this.form.display_name = payload.device ? (payload.device.display_name_input || payload.device.display_name || "") : "";
            this.form.room_name = payload.device ? (payload.device.room_name || "") : "";
            const relayNameFields = Array.isArray(payload.device?.relay_name_fields) ? payload.device.relay_name_fields : [];
            this.form.relay_names = relayNameFields.reduce((acc, field) => {
                acc[field.key] = field.value || "";
                return acc;
            }, {});
            const chartMetrics = Array.isArray(payload.chart_metrics) ? payload.chart_metrics : [];
            const allowedMetrics = new Set(chartMetrics.map((item) => item.key));
            this.chartForm.metrics = this.chartForm.metrics.filter((item) => allowedMetrics.has(item));
            if (!this.chartForm.metrics.length && chartMetrics.length) {
                this.chartForm.metrics = chartMetrics.slice(0, 2).map((item) => item.key);
            }
            if (!this.autoChartLoaded && this.chartForm.metrics.length) {
                this.autoChartLoaded = true;
                setTimeout(() => this.loadChart(), 120);
            }
        }
    },
    watch: {
        msg: {
            immediate: true,
            handler(value) {
                const payload = value?.payload || {};
                if (payload.kind === "detail") {
                    this.removedMessage = "";
                    this.applyDetailPayload(payload);
                    return;
                }
                if (payload.kind === "chart") {
                    if (payload.chart && payload.chart.device_id && payload.chart.device_id !== this.currentDeviceId()) {
                        return;
                    }
                    this.chart = payload.chart || {};
                    return;
                }
                if (payload.kind === "detail_removed") {
                    this.detail = { room_options: [], sensor_metrics: [], chart_metrics: [], chart_unavailable_reason: "" };
                    this.chart = {};
                    this.removedMessage = payload.message || "Gerät entfernt.";
                }
            }
        }
    },
    mounted() {
        setTimeout(() => this.reload(), 140);
    }
}
</script>

${dashboardSharedStyle}
`;

const chartsTemplate = `
<template>
    <div class="sh-page">
        <div class="sh-toolbar">
            <div class="sh-toolbar-actions">
                <button class="sh-btn sh-btn-muted" @click="refreshDevices">Neu laden</button>
                ${renderDashboardNavHtml()}
            </div>
        </div>

        <div class="sh-panel">
            <div class="sh-form-row">
                <div class="sh-field">
                    <label>Gerät</label>
                    <select class="sh-select" v-model="selectedDeviceId">
                        <option value="">Bitte wählen</option>
                        <option v-for="device in deviceOptions" :key="device.device_id" :value="device.device_id">
                            {{ device.display_name }}
                        </option>
                    </select>
                </div>
                <div class="sh-field">
                    <label>Zeitraum</label>
                    <select class="sh-select" v-model="chartForm.range">
                        <option value="1h">1h</option>
                        <option value="6h">6h</option>
                        <option value="24h">24h</option>
                        <option value="7d">7d</option>
                        <option value="30d">30d</option>
                    </select>
                </div>
                <div class="sh-field">
                    <label>Schrittweite</label>
                    <select class="sh-select" v-model="chartForm.step">
                        <option value="auto">Auto</option>
                        <option value="raw">Rohdaten</option>
                        <option value="10s">10s</option>
                        <option value="30s">30s</option>
                        <option value="1m">1m</option>
                        <option value="5m">5m</option>
                        <option value="15m">15m</option>
                        <option value="1h">1h</option>
                    </select>
                </div>
            </div>
            <div class="sh-checkbox-list" v-if="metricOptions.length">
                <label class="sh-secondary-pill" v-for="metric in metricOptions" :key="'chart-metric-' + metric.key">
                    <input type="checkbox" :value="metric.key" v-model="chartForm.metrics" />
                    {{ metric.label }}
                </label>
            </div>
            <div class="sh-control-row">
                <button class="sh-btn" @click="loadChart" :disabled="!selectedDeviceId">Diagramm laden</button>
                <a class="sh-btn" v-if="selectedDeviceId" :href="'/dashboard/geraet?device=' + encodeURIComponent(selectedDeviceId)">Zum Gerätedetail</a>
            </div>
            <div class="sh-empty" v-if="!deviceOptions.length">Noch keine chartbaren Influx-Zeitreihen sichtbar.</div>
        </div>

        <template v-if="chart.panels && chart.panels.length">
            <div class="sh-chart-shell">
                <div class="sh-chart-meta">
                    <span class="sh-chip">Gerät {{ chart.title }}</span>
                    <span class="sh-chip">Zeitraum {{ chart.range_label }}</span>
                    <span class="sh-chip">Raster {{ chart.step_label }}</span>
                    <span class="sh-chip" v-if="chart.metric_summary">Kurven {{ chart.metric_summary }}</span>
                </div>
                <div class="sh-chart-note" v-if="chart.notice" :class="chart.status === 'sparse' ? 'is-warning' : ''">
                    <strong>Hinweis</strong>
                    <p>{{ chart.notice }}</p>
                </div>
                <div class="sh-chart-stack">
                    <section class="sh-chart-panel" v-for="panel in chart.panels" :key="'chart-panel-' + panel.unit_key">
                        <div class="sh-chart-panel-head">
                            <div class="sh-chart-panel-copy">
                                <strong>{{ panel.axis_label }}</strong>
                                <span class="sh-muted">{{ panel.series.length }} Kurve(n)<template v-if="panel.series.length > 1"> · {{ panel.metric_summary }}</template></span>
                            </div>
                            <div class="sh-chart-panel-stats">
                                <span class="sh-chip">min {{ panel.min_label }}</span>
                                <span class="sh-chip">max {{ panel.max_label }}</span>
                                <span class="sh-chip">{{ panel.point_count }} Punkte</span>
                            </div>
                        </div>
                        <svg class="sh-chart-svg" viewBox="0 0 640 280" width="100%" height="280" role="img" :aria-label="'Sensorverlauf ' + panel.axis_label">
                            <g v-for="tick in panel.y_ticks" :key="'chart-y-' + panel.unit_key + '-' + tick.value_num">
                                <line x1="72" :y1="tick.y" x2="618" :y2="tick.y" stroke="rgba(185, 199, 213, 0.16)" stroke-dasharray="4 4" />
                                <text x="64" :y="tick.y + 4" fill="#8fa5ba" font-size="12" text-anchor="end">{{ tick.label }}</text>
                            </g>
                            <g v-for="tick in chart.x_ticks" :key="'chart-x-' + panel.unit_key + '-' + tick.ts_ms">
                                <line :x1="tick.x" y1="28" :x2="tick.x" y2="212" stroke="rgba(185, 199, 213, 0.1)" stroke-dasharray="4 4" />
                                <text :x="tick.x" y="234" fill="#8fa5ba" font-size="12" text-anchor="middle">{{ tick.label }}</text>
                            </g>
                            <line x1="72" y1="28" x2="72" y2="212" stroke="#b9c7d5" stroke-width="1" />
                            <line x1="72" y1="212" x2="618" y2="212" stroke="#b9c7d5" stroke-width="1" />
                            <text x="22" y="120" fill="#9fc2de" font-size="12" text-anchor="middle" transform="rotate(-90 22 120)">{{ panel.axis_label }}</text>
                            <text x="345" y="258" fill="#9fc2de" font-size="12" text-anchor="middle">{{ chart.x_axis_label }}</text>
                            <path
                                v-for="series in panel.series"
                                :key="'chart-series-' + panel.unit_key + '-' + series.metric"
                                :d="seriesPath(series)"
                                fill="none"
                                :stroke="series.color"
                                :stroke-width="seriesStrokeWidth(series)"
                                stroke-linecap="round"
                                stroke-linejoin="round" />
                            <g v-for="series in panel.series" :key="'chart-points-' + panel.unit_key + '-' + series.metric">
                                <circle
                                    v-for="point in series.points"
                                    :key="'chart-point-' + series.metric + '-' + point.ts_ms + '-' + point.value_num"
                                    :cx="point.x"
                                    :cy="point.y"
                                    :r="pointRadius(series)"
                                    :fill="series.color"
                                    stroke="#0f161f"
                                    stroke-width="1.4">
                                    <title>{{ pointTitle(series, point) }}</title>
                                </circle>
                            </g>
                        </svg>
                        <div class="sh-chart-axis-row">
                            <span>{{ chart.x_axis_label }}</span>
                            <span>{{ chart.start_label }} bis {{ chart.end_label }}</span>
                        </div>
                        <div class="sh-chart-legend">
                            <span v-for="series in panel.series" :key="'chart-legend-' + panel.unit_key + '-' + series.metric">
                                <strong><i :style="{ background: series.color }"></i>{{ series.label }}</strong>
                                <small>Aktuell {{ series.last_value_text }} · {{ series.point_count }} Punkte<template v-if="!series.has_drawable_line"> · Einzelpunkt</template></small>
                            </span>
                        </div>
                    </section>
                </div>
            </div>
        </template>
        <template v-else-if="chart.message">
            <div class="sh-chart-meta" v-if="chart.title || chart.range_label || chart.step_label || chart.metric_summary">
                <span class="sh-chip" v-if="chart.title">Gerät {{ chart.title }}</span>
                <span class="sh-chip" v-if="chart.range_label">Zeitraum {{ chart.range_label }}</span>
                <span class="sh-chip" v-if="chart.step_label">Raster {{ chart.step_label }}</span>
                <span class="sh-chip" v-if="chart.metric_summary">Kurven {{ chart.metric_summary }}</span>
            </div>
            <div class="sh-chart-state" :class="chartStateClass(chart.status)">
                <strong>{{ chartStateTitle(chart.status) }}</strong>
                <p>{{ chart.message }}</p>
            </div>
        </template>
    </div>
</template>

<script>
export default {
    data() {
        return {
            dataset: { devices: [] },
            chart: {},
            selectedDeviceId: "",
            chartForm: { range: "24h", step: "auto", metrics: [] }
        };
    },
    computed: {
        deviceOptions() {
            return Array.isArray(this.dataset.devices) ? this.dataset.devices : [];
        },
        selectedDevice() {
            return this.deviceOptions.find((device) => device.device_id === this.selectedDeviceId) || null;
        },
        metricOptions() {
            return this.selectedDevice && Array.isArray(this.selectedDevice.chart_metrics)
                ? this.selectedDevice.chart_metrics
                : [];
        }
    },
    methods: {
        refreshDevices() {
            this.send({ payload: { view: "charts", action: "load" } });
        },
        loadChart() {
            this.send({
                payload: {
                    view: "charts",
                    action: "chart",
                    chart_target: "charts",
                    device_id: this.selectedDeviceId,
                    device_label: this.selectedDevice ? this.selectedDevice.display_name : this.selectedDeviceId,
                    metrics: this.chartForm.metrics,
                    range: this.chartForm.range,
                    step: this.chartForm.step
                }
            });
        },
        seriesPath(series) {
            if (!series || !Array.isArray(series.points) || !series.points.length) {
                return "";
            }
            return series.points
                .map((point, index) => (index === 0 ? "M" : "L") + point.x + " " + point.y)
                .join(" ");
        },
        seriesStrokeWidth(series) {
            if (!series || !series.point_count) return 2.2;
            return series.point_count <= 8 ? 2.8 : series.point_count <= 24 ? 2.4 : 2.2;
        },
        pointRadius(series) {
            if (!series || !series.point_count) return 3.2;
            if (!series.has_drawable_line) return 4.4;
            return series.point_count <= 8 ? 3.6 : series.point_count <= 24 ? 3.1 : 2.7;
        },
        chartStateClass(status) {
            return {
                "is-invalid": status === "invalid_selection",
                "is-no-data": status === "no_data",
                "is-error": status === "query_error" || status === "render_error",
                "is-warning": status === "sparse"
            };
        },
        chartStateTitle(status) {
            if (status === "invalid_selection") return "Ungültige Auswahl";
            if (status === "no_data") return "Keine Influx-Daten";
            if (status === "query_error") return "Influx-Fehler";
            if (status === "render_error") return "Darstellungsproblem";
            if (status === "sparse") return "Wenige Datenpunkte";
            return "Diagrammstatus";
        },
        pointTitle(series, point) {
            return [series && series.label, this.formatTimestamp(point && point.ts_ms), point && point.value_text].filter(Boolean).join(" · ");
        },
        formatTimestamp(tsMs) {
            if (!Number.isFinite(tsMs)) return "";
            const date = new Date(tsMs);
            if (Number.isNaN(date.getTime())) return "";
            return date.toLocaleString("de-DE", {
                day: "2-digit",
                month: "2-digit",
                hour: "2-digit",
                minute: "2-digit",
                second: "2-digit"
            });
        }
    },
    watch: {
        msg: {
            immediate: true,
            handler(value) {
                const payload = value?.payload || {};
                if (payload.page && payload.page.key === "charts") {
                    this.dataset = payload;
                    const knownDeviceIds = Array.isArray(payload.devices) ? payload.devices.map((device) => device.device_id) : [];
                    if (this.selectedDeviceId && !knownDeviceIds.includes(this.selectedDeviceId)) {
                        this.selectedDeviceId = "";
                    }
                    if (!this.selectedDeviceId && Array.isArray(payload.devices) && payload.devices.length) {
                        this.selectedDeviceId = payload.devices[0].device_id;
                    }
                    if (!this.chartForm.metrics.length && this.selectedDevice && Array.isArray(this.selectedDevice.chart_metrics)) {
                        this.chartForm.metrics = this.selectedDevice.chart_metrics.slice(0, 2).map((item) => item.key);
                    }
                    return;
                }
                if (payload.kind === "chart") {
                    if (payload.chart && payload.chart.device_id && payload.chart.device_id !== this.selectedDeviceId) {
                        return;
                    }
                    this.chart = payload.chart || {};
                }
            }
        },
        selectedDeviceId() {
            this.chart = {};
            if (this.selectedDevice && Array.isArray(this.selectedDevice.chart_metrics)) {
                this.chartForm.metrics = this.selectedDevice.chart_metrics.slice(0, 2).map((item) => item.key);
            } else {
                this.chartForm.metrics = [];
            }
        }
    },
    mounted() {
        setTimeout(() => this.refreshDevices(), 120);
    }
}
</script>

${dashboardSharedStyle}
`;

const weatherTemplate = `
<template>
    <div class="sh-page">
        <div class="sh-toolbar">
            <div class="sh-toolbar-actions">
                <button class="sh-btn sh-btn-muted" @click="reload">Neu laden</button>
                ${renderDashboardNavHtml()}
            </div>
        </div>

        <div class="sh-weather-grid">
            <section class="sh-panel">
                <h3>Quelle</h3>
                <div class="sh-form-row">
                    <div class="sh-field">
                        <label>Provider</label>
                        <input class="sh-input" v-model="form.provider" type="text" />
                    </div>
                    <div class="sh-field">
                        <label>Ort</label>
                        <input class="sh-input" v-model="form.location_label" type="text" />
                    </div>
                </div>
                <div class="sh-form-row">
                    <div class="sh-field">
                        <label>Breite</label>
                        <input class="sh-input" v-model="form.latitude" type="number" step="0.0001" />
                    </div>
                    <div class="sh-field">
                        <label>Länge</label>
                        <input class="sh-input" v-model="form.longitude" type="number" step="0.0001" />
                    </div>
                    <div class="sh-field">
                        <label>Intervall (Minuten)</label>
                        <input class="sh-input" v-model="form.poll_interval_minutes" type="number" min="1" />
                    </div>
                    <div class="sh-field">
                        <label>Einheiten</label>
                        <select class="sh-select" v-model="form.units">
                            <option value="metric">metric</option>
                            <option value="imperial">imperial</option>
                        </select>
                    </div>
                </div>
                <div class="sh-control-row">
                    <label class="sh-secondary-pill"><input type="checkbox" v-model="form.enabled" /> Wetter aktivieren</label>
                    <button class="sh-btn" @click="save">Speichern</button>
                </div>
                <p class="sh-muted">{{ note }}</p>
            </section>

            <section class="sh-panel">
                <h3>Stand</h3>
                <div class="sh-value-list">
                    <div class="sh-value-row"><span>Zuletzt aktualisiert</span><strong>{{ payload.updated_at || '-' }}</strong></div>
                    <div class="sh-value-row"><span>API-Basis</span><strong>{{ payload.api_base_url || '-' }}</strong></div>
                    <div class="sh-value-row"><span>Status</span><strong>{{ payload.enabled ? 'aktiviert' : 'deaktiviert' }}</strong></div>
                </div>
                <p class="sh-muted">Solange der Server noch keinen produktiven Wetter-Feed schreibt, bleibt diese Seite eine ehrliche Konfigurationsstufe.</p>
            </section>
        </div>
    </div>
</template>

<script>
export default {
    data() {
        return {
            payload: {},
            form: {
                provider: "open-meteo",
                location_label: "",
                latitude: "",
                longitude: "",
                poll_interval_minutes: 15,
                units: "metric",
                enabled: false
            },
            note: ""
        };
    },
    methods: {
        reload() {
            this.send({ payload: { view: "weather", action: "load" } });
        },
        save() {
            this.send({ payload: Object.assign({ view: "weather", action: "save" }, this.form) });
        }
    },
    watch: {
        msg: {
            immediate: true,
            handler(value) {
                const payload = value?.payload || {};
                if (payload.kind === "weather") {
                    this.payload = payload;
                    this.form = {
                        provider: payload.provider || "open-meteo",
                        location_label: payload.location_label || "",
                        latitude: payload.latitude ?? "",
                        longitude: payload.longitude ?? "",
                        poll_interval_minutes: payload.poll_interval_minutes || 15,
                        units: payload.units || "metric",
                        enabled: !!payload.enabled
                    };
                    this.note = payload.note || "";
                }
            }
        }
    },
    mounted() {
        setTimeout(() => this.reload(), 120);
    }
}
</script>

${dashboardSharedStyle}
`;

const headerTemplate = `
<template>
    <div class="sh-global-strip-shell">
        <div class="sh-global-strip">
            <div class="sh-header-spacer" aria-hidden="true"></div>
            <div class="sh-header-notice" :class="{ 'is-open': noticeOpen }">
                <button
                    class="sh-header-notice-button"
                    type="button"
                    aria-haspopup="dialog"
                    :aria-expanded="noticeOpen ? 'true' : 'false'"
                    @click.stop="toggleNotice">
                    <i class="mdi mdi-bell-outline" aria-hidden="true"></i>
                    <span class="sh-header-notice-count">{{ noticeCount }}</span>
                </button>
                <div class="sh-header-notice-panel" v-if="noticeOpen">
                    <strong>Benachrichtigungen</strong>
                    <p>{{ notificationLabel }}</p>
                </div>
            </div>
            <div class="sh-header-weather" :class="'is-' + weatherMode" :title="weatherTooltip">
                <span class="sh-header-weather-icon" :class="'is-' + weatherMode" aria-hidden="true">
                    <i :class="'mdi ' + weatherIcon"></i>
                </span>
                <span class="sh-header-weather-copy">
                    <strong>{{ weatherHeadline }}</strong>
                    <span class="sh-header-weather-meta">
                        <span
                            class="sh-header-weather-token"
                            v-for="(token, index) in weatherTokens"
                            :key="token + '-' + index">
                            {{ token }}
                        </span>
                    </span>
                </span>
            </div>
        </div>
    </div>
</template>

<script>
export default {
    data() {
        return {
            payload: {
                weather: {},
                notifications: {
                    count: 0,
                    label: "Noch kein Benachrichtigungskanal in V1."
                }
            },
            noticeOpen: false,
            nowTs: Date.now()
        };
    },
    computed: {
        weather() {
            return this.payload?.weather || {};
        },
        weatherMode() {
            return this.weather.icon_mode || "inactive";
        },
        weatherIcon() {
            if (this.weatherMode === "configured") return "mdi-weather-partly-cloudy";
            if (this.weatherMode === "pending") return "mdi-weather-cloudy-alert";
            return "mdi-weather-cloudy-off";
        },
        weatherHeadline() {
            const location = this.weather.location_label || this.weather.coords_label || "";
            if (this.weather.enabled && location) return location;
            if (this.weather.enabled) return "Wetter aktiv";
            if (location) return location;
            return "Wetter inaktiv";
        },
        weatherAgeText() {
            return this.formatAgeLabel(this.weather.updated_at, this.weather.age_fallback || "Stand -");
        },
        weatherTokens() {
            const tokens = [];
            const provider = this.weather.provider_label || "";
            if (provider && provider !== this.weatherHeadline) tokens.push(provider);
            if (this.weather.enabled && this.weather.poll_label) tokens.push(this.weather.poll_label);
            if (this.weather.enabled && this.weather.status_note) tokens.push(this.weather.status_note);
            tokens.push(this.weatherAgeText);
            return tokens.filter(Boolean).slice(0, 4);
        },
        weatherTooltip() {
            return this.weather.tooltip || "";
        },
        noticeCount() {
            const count = Number(this.payload?.notifications?.count);
            return Number.isFinite(count) ? Math.max(0, count) : 0;
        },
        notificationLabel() {
            return this.payload?.notifications?.label || "Noch kein Benachrichtigungskanal in V1.";
        }
    },
    methods: {
        reload() {
            this.send({ payload: { view: "header", action: "load" } });
        },
        toggleNotice() {
            this.noticeOpen = !this.noticeOpen;
        },
        handleDocumentClick(event) {
            const target = event?.target;
            if (target && typeof target.closest === "function" && target.closest(".sh-header-notice")) {
                return;
            }
            this.noticeOpen = false;
        },
        parseTimestamp(value) {
            const text = typeof value === "string" ? value.trim() : "";
            if (!text) return null;
            const normalized = text.includes("T") || text.endsWith("Z")
                ? text
                : text.replace(" ", "T") + "Z";
            const tsMs = Date.parse(normalized);
            return Number.isFinite(tsMs) ? tsMs : null;
        },
        formatDateLabel(value) {
            const tsMs = this.parseTimestamp(value);
            if (tsMs === null) return "-";
            return new Intl.DateTimeFormat("de-DE", {
                day: "2-digit",
                month: "2-digit",
                hour: "2-digit",
                minute: "2-digit"
            }).format(new Date(tsMs));
        },
        formatAgeLabel(value, fallback) {
            const tsMs = this.parseTimestamp(value);
            if (tsMs === null) return fallback;
            const ageMs = this.nowTs - tsMs;
            if (!Number.isFinite(ageMs) || ageMs < 0) {
                return "Stand " + this.formatDateLabel(value);
            }
            const ageSeconds = Math.max(1, Math.floor(ageMs / 1000));
            if (ageSeconds < 60) return "Stand " + ageSeconds + " s";
            const ageMinutes = Math.floor(ageSeconds / 60);
            if (ageMinutes < 60) return "Stand " + ageMinutes + " min";
            const ageHours = Math.floor(ageMinutes / 60);
            if (ageHours < 24) return "Stand " + ageHours + " h";
            const ageDays = Math.floor(ageHours / 24);
            if (ageDays < 7) return "Stand " + ageDays + " d";
            return "Stand " + this.formatDateLabel(value);
        }
    },
    watch: {
        msg: {
            immediate: true,
            handler(value) {
                const payload = value?.payload || {};
                if (payload.kind === "header") {
                    this.payload = payload;
                }
            }
        }
    },
    mounted() {
        setTimeout(() => this.reload(), 120);
        this._timer = setInterval(() => {
            this.nowTs = Date.now();
        }, 30000);
        document.addEventListener("click", this.handleDocumentClick);
    },
    unmounted() {
        if (this._timer) clearInterval(this._timer);
        document.removeEventListener("click", this.handleDocumentClick);
    }
}
</script>

<style>
    .v-main > .nrdb-layout--grid.nrdb-ui-page {
        padding-top: 3.15rem;
    }

    .sh-global-strip-shell {
        position: fixed;
        top: 4.15rem;
        left: 0;
        right: 0;
        z-index: 6;
        padding: 0 0.75rem;
        pointer-events: none;
    }

    .sh-global-strip {
        position: relative;
        display: flex;
        align-items: center;
        justify-content: flex-end;
        min-height: 2.5rem;
        max-width: min(72rem, calc(100vw - 2rem));
        margin-left: auto;
    }

    .sh-header-spacer {
        flex: 1 1 auto;
    }

    .sh-header-weather {
        display: inline-flex;
        align-items: center;
        gap: 0.58rem;
        min-width: 0;
        max-width: min(32rem, calc(100vw - 9rem));
        padding: 0.34rem 0.72rem 0.34rem 0.56rem;
        border-radius: 999px;
        border: 1px solid rgba(54, 72, 90, 0.84);
        background: linear-gradient(180deg, rgba(23, 34, 46, 0.94), rgba(12, 18, 26, 0.98));
        box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.04);
        pointer-events: auto;
    }

    .sh-header-weather.is-configured {
        border-color: rgba(181, 152, 86, 0.68);
        background: linear-gradient(180deg, rgba(49, 40, 20, 0.94), rgba(25, 18, 8, 0.98));
    }

    .sh-header-weather.is-pending {
        border-color: rgba(81, 104, 126, 0.76);
    }

    .sh-header-weather-icon {
        flex: 0 0 auto;
        width: 1.9rem;
        height: 1.9rem;
        display: inline-flex;
        align-items: center;
        justify-content: center;
        border-radius: 999px;
        background: rgba(19, 28, 38, 0.76);
        border: 1px solid rgba(63, 81, 99, 0.82);
        color: #8ca0b3;
    }

    .sh-header-weather-icon i {
        font-size: 1.1rem;
    }

    .sh-header-weather-icon.is-configured {
        color: #ffd869;
        border-color: rgba(232, 191, 93, 0.76);
        background: rgba(74, 55, 18, 0.92);
    }

    .sh-header-weather-icon.is-configured i {
        animation: shHeaderWeatherFloat 3s ease-in-out infinite;
    }

    .sh-header-weather-icon.is-pending {
        color: #bfd0e2;
    }

    .sh-header-weather-icon.is-pending i {
        animation: shHeaderWeatherPulse 1.9s ease-in-out infinite;
    }

    .sh-header-weather-copy {
        min-width: 0;
        display: grid;
        gap: 0.08rem;
    }

    .sh-header-weather-copy strong {
        font-size: 0.82rem;
        line-height: 1.05;
        white-space: nowrap;
        overflow: hidden;
        text-overflow: ellipsis;
    }

    .sh-header-weather-meta {
        display: flex;
        align-items: center;
        gap: 0.36rem;
        min-width: 0;
        font-size: 0.72rem;
        color: rgba(195, 210, 224, 0.88);
        white-space: nowrap;
        overflow: hidden;
    }

    .sh-header-weather-token {
        min-width: 0;
        overflow: hidden;
        text-overflow: ellipsis;
        white-space: nowrap;
    }

    .sh-header-weather-token + .sh-header-weather-token::before {
        content: "•";
        margin-right: 0.36rem;
        color: rgba(126, 147, 166, 0.84);
    }

    .sh-header-notice {
        position: absolute;
        left: 50%;
        top: 50%;
        transform: translate(-50%, -50%);
        z-index: 3;
        pointer-events: auto;
    }

    .sh-header-notice-button {
        display: inline-flex;
        align-items: center;
        gap: 0.4rem;
        min-width: 4rem;
        justify-content: center;
        padding: 0.34rem 0.68rem;
        border: 1px solid rgba(59, 77, 96, 0.84);
        border-radius: 999px;
        background: linear-gradient(180deg, rgba(20, 30, 41, 0.94), rgba(11, 17, 25, 0.98));
        color: #d3e1ef;
        box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.03);
    }

    .sh-header-notice-button:hover {
        border-color: rgba(91, 114, 137, 0.84);
    }

    .sh-header-notice-count {
        min-width: 1.3rem;
        height: 1.3rem;
        padding: 0 0.28rem;
        border-radius: 999px;
        background: rgba(44, 59, 73, 0.96);
        border: 1px solid rgba(87, 106, 126, 0.88);
        display: inline-flex;
        align-items: center;
        justify-content: center;
        font-size: 0.72rem;
        font-weight: 700;
        line-height: 1;
    }

    .sh-header-notice-panel {
        position: absolute;
        left: 50%;
        top: calc(100% + 0.45rem);
        transform: translateX(-50%);
        width: 15rem;
        padding: 0.7rem 0.78rem;
        border-radius: 16px;
        border: 1px solid rgba(53, 71, 90, 0.92);
        background: linear-gradient(180deg, rgba(20, 30, 41, 0.98), rgba(11, 17, 24, 0.99));
        box-shadow: 0 16px 34px rgba(0, 0, 0, 0.26);
    }

    .sh-header-notice-panel strong {
        display: block;
        font-size: 0.83rem;
        margin-bottom: 0.18rem;
    }

    .sh-header-notice-panel p {
        margin: 0;
        font-size: 0.77rem;
        color: rgba(196, 211, 224, 0.88);
        line-height: 1.35;
    }

    @keyframes shHeaderWeatherFloat {
        0%,
        100% {
            transform: translateY(0);
        }
        50% {
            transform: translateY(-1px);
        }
    }

    @keyframes shHeaderWeatherPulse {
        0%,
        100% {
            opacity: 0.86;
            transform: scale(1);
        }
        50% {
            opacity: 1;
            transform: scale(1.04);
        }
    }

    @media (max-width: 1100px) {
        .sh-header-notice {
            left: calc(50% - 1rem);
        }

        .sh-header-weather {
            max-width: 18rem;
        }

        .sh-header-weather-token:nth-child(1) {
            display: none;
        }
    }

    @media (max-width: 760px) {
        .v-main > .nrdb-layout--grid.nrdb-ui-page {
            padding-top: 2.95rem;
        }

        .sh-global-strip-shell {
            top: 3.95rem;
            padding: 0 0.5rem;
        }

        .sh-header-notice {
            display: none;
        }

        .sh-header-weather {
            max-width: calc(100vw - 1rem);
            padding-right: 0.58rem;
        }

        .sh-header-weather-copy strong {
            font-size: 0.78rem;
        }

        .sh-header-weather-meta {
            font-size: 0.69rem;
        }

        .sh-header-weather-token:nth-child(2),
        .sh-header-weather-token:nth-child(3) {
            display: none;
        }
    }
</style>
`;

const automationsTemplate = `
<template>
    <div class="sh-page">
        <div class="sh-toolbar">
            <div class="sh-toolbar-actions">
                <button class="sh-btn sh-btn-muted" @click="reload">Neu laden</button>
                ${renderDashboardNavHtml()}
            </div>
        </div>

        <section class="sh-panel">
            <h3>{{ form.id ? 'Automation bearbeiten' : 'Neue Automation' }}</h3>
            <div class="sh-form-row">
                <div class="sh-field">
                    <label>Name</label>
                    <input class="sh-input" v-model="form.name" type="text" />
                </div>
                <div class="sh-field">
                    <label>Trigger</label>
                    <select class="sh-select" v-model="form.trigger_type">
                        <option value="time">Zeit</option>
                        <option value="sensor">Sensor</option>
                    </select>
                </div>
            </div>
            <div class="sh-form-row" v-if="form.trigger_type === 'time'">
                <div class="sh-field">
                    <label>Uhrzeit</label>
                    <input class="sh-input" v-model="form.time_at" type="time" />
                </div>
            </div>
            <div class="sh-form-row" v-else>
                <div class="sh-field">
                    <label>Sensor-Gerät</label>
                    <select class="sh-select" v-model="form.sensor_device_id">
                        <option value="">Bitte wählen</option>
                        <option v-for="item in sensorDevices" :key="'sensor-device-' + item.device_id" :value="item.device_id">
                            {{ item.display_name }}
                        </option>
                    </select>
                </div>
                <div class="sh-field">
                    <label>Metrik</label>
                    <select class="sh-select" v-model="form.sensor_metric">
                        <option value="">Bitte wählen</option>
                        <option v-for="metric in selectedSensorMetrics" :key="'sensor-metric-' + metric.key" :value="metric.key">
                            {{ metric.label }}
                        </option>
                    </select>
                </div>
                <div class="sh-field">
                    <label>Operator</label>
                    <select class="sh-select" v-model="form.sensor_operator">
                        <option value=">=">>=</option>
                        <option value=">">></option>
                        <option value="<"><</option>
                        <option value="<="><=</option>
                        <option value="==">==</option>
                    </select>
                </div>
                <div class="sh-field">
                    <label>Schwellwert</label>
                    <input class="sh-input" v-model="form.sensor_threshold" type="number" step="0.1" />
                </div>
            </div>
            <div class="sh-form-row">
                <div class="sh-field">
                    <label>Ziel-Gerät</label>
                    <select class="sh-select" v-model="form.action_device_id">
                        <option value="">Bitte wählen</option>
                        <option v-for="item in actuatorDevices" :key="'action-device-' + item.device_id" :value="item.device_id">
                            {{ item.display_name }}
                        </option>
                    </select>
                </div>
                <div class="sh-field">
                    <label>Aktion</label>
                    <select class="sh-select" v-model="form.action_code">
                        <option value="">Bitte wählen</option>
                        <option v-for="action in selectedActionOptions" :key="'action-code-' + action.code" :value="action.code">
                            {{ action.label }}
                        </option>
                    </select>
                </div>
            </div>
            <div class="sh-form-row">
                <div class="sh-field">
                    <label>Notiz</label>
                    <textarea class="sh-textarea" v-model="form.notes"></textarea>
                </div>
            </div>
            <div class="sh-control-row">
                <label class="sh-secondary-pill"><input type="checkbox" v-model="form.is_enabled" /> aktiviert</label>
                <button class="sh-btn" @click="save">Speichern</button>
                <button class="sh-btn sh-btn-muted" @click="resetForm">Leeren</button>
            </div>
            <p class="sh-muted">Wettertrigger bleiben absichtlich gesperrt, weil der Server aktuell nur Wetterkonfiguration, aber keinen belastbaren Wetterzustand hält.</p>
        </section>

        <section class="sh-panel">
            <h3>Aktive Regeln</h3>
            <table class="sh-technical-table" v-if="automationRows.length">
                <thead>
                    <tr>
                        <th>Name</th>
                        <th>Trigger</th>
                        <th>Aktion</th>
                        <th>Status</th>
                        <th>Zuletzt</th>
                        <th></th>
                    </tr>
                </thead>
                <tbody>
                    <tr v-for="item in automationRows" :key="'automation-' + item.id">
                        <td>{{ item.name }}</td>
                        <td>{{ item.trigger_label }}</td>
                        <td>{{ item.action_label }}</td>
                        <td>{{ item.is_enabled ? 'aktiv' : 'pausiert' }}</td>
                        <td>{{ item.last_executed_at || '-' }}</td>
                        <td>
                            <div class="sh-control-row">
                                <button class="sh-btn sh-btn-muted" @click="editAutomation(item)">Bearbeiten</button>
                                <button class="sh-btn sh-btn-muted" @click="toggleAutomation(item)">{{ item.is_enabled ? 'Pause' : 'Aktiv' }}</button>
                                <button class="sh-btn sh-btn-danger" @click="deleteAutomation(item)">Löschen</button>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            <div class="sh-empty" v-else>Noch keine Automationen angelegt.</div>
        </section>
    </div>
</template>

<script>
export default {
    data() {
        return {
            payload: { sensor_devices: [], actuator_devices: [], automations: [] },
            form: {
                id: null,
                name: "",
                trigger_type: "time",
                time_at: "07:00",
                sensor_device_id: "",
                sensor_metric: "",
                sensor_operator: ">=",
                sensor_threshold: "",
                action_device_id: "",
                action_code: "",
                notes: "",
                is_enabled: true
            }
        };
    },
    computed: {
        sensorDevices() {
            return Array.isArray(this.payload.sensor_devices) ? this.payload.sensor_devices : [];
        },
        actuatorDevices() {
            return Array.isArray(this.payload.actuator_devices) ? this.payload.actuator_devices : [];
        },
        automationRows() {
            return Array.isArray(this.payload.automations) ? this.payload.automations : [];
        },
        selectedSensorMetrics() {
            const current = this.sensorDevices.find((item) => item.device_id === this.form.sensor_device_id);
            return current && Array.isArray(current.metrics) ? current.metrics : [];
        },
        selectedActionOptions() {
            const current = this.actuatorDevices.find((item) => item.device_id === this.form.action_device_id);
            return current && Array.isArray(current.actions) ? current.actions : [];
        }
    },
    methods: {
        reload() {
            this.send({ payload: { view: "automations", action: "load" } });
        },
        resetForm() {
            this.form = {
                id: null,
                name: "",
                trigger_type: "time",
                time_at: "07:00",
                sensor_device_id: "",
                sensor_metric: "",
                sensor_operator: ">=",
                sensor_threshold: "",
                action_device_id: "",
                action_code: "",
                notes: "",
                is_enabled: true
            };
        },
        save() {
            this.send({ payload: Object.assign({ view: "automations", action: "save" }, this.form) });
        },
        editAutomation(item) {
            this.form = Object.assign({}, item.form_values);
        },
        toggleAutomation(item) {
            this.send({ payload: { view: "automations", action: "toggle", id: item.id, is_enabled: !item.is_enabled } });
        },
        deleteAutomation(item) {
            this.send({ payload: { view: "automations", action: "delete", id: item.id } });
        }
    },
    watch: {
        msg: {
            immediate: true,
            handler(value) {
                const payload = value?.payload || {};
                if (payload.kind === "automations") {
                    this.payload = payload;
                }
            }
        }
    },
    mounted() {
        setTimeout(() => this.reload(), 120);
    }
}
</script>

${dashboardSharedStyle}
`;

const configTemplate = `
<template>
    <div class="sh-page">
        <div class="sh-toolbar">
            <div class="sh-toolbar-actions">
                <button class="sh-btn sh-btn-muted" @click="reload">Neu laden</button>
                ${renderDashboardNavHtml()}
            </div>
        </div>

        <div class="sh-config-grid">
            <section class="sh-panel">
                <h3>Server-Konfiguration</h3>
                <p class="sh-muted">Schreibbarer Serverpfad in V1: Wetter-Setup.</p>
                <div class="sh-value-list">
                    <div class="sh-value-row"><span>Provider</span><strong>{{ payload.weather.provider || '-' }}</strong></div>
                    <div class="sh-value-row"><span>Ort</span><strong>{{ payload.weather.location_label || '-' }}</strong></div>
                    <div class="sh-value-row"><span>Abfrage</span><strong>{{ payload.weather.poll_label || '-' }}</strong></div>
                </div>
                <div class="sh-control-row">
                    <a class="sh-btn" href="/dashboard/wetter">Wetter bearbeiten</a>
                </div>
                <p class="sh-muted">Weitere server_settings bleiben aktuell technische SQLite-Werte ohne verdrahtete Live-Wirkung.</p>
            </section>
            <section class="sh-panel">
                <h3>Geräte-Konfiguration</h3>
                <p class="sh-muted">Schreibbar sind Anzeigename, Raumzuordnung und UI-Relaisnamen im Gerätedetail.</p>
                <div class="sh-form-row">
                    <div class="sh-field">
                        <label>Gerät</label>
                        <select class="sh-select" v-model="selectedDeviceId">
                            <option value="">Bitte wählen</option>
                            <option v-for="device in payload.devices" :key="'config-device-' + device.device_id" :value="device.device_id">
                                {{ device.display_name }}
                            </option>
                        </select>
                    </div>
                </div>
                <div class="sh-control-row">
                    <a class="sh-btn" v-if="selectedDeviceId" :href="'/dashboard/geraet?device=' + encodeURIComponent(selectedDeviceId)">Gerät öffnen</a>
                    <a class="sh-btn sh-btn-muted" href="/dashboard/geraete">Geräteübersicht</a>
                </div>
                <p class="sh-muted">Gerätemeldungen aus cfg/report bleiben im Gerätedetail sichtbar; UI-Namen werden getrennt nur serverseitig gespeichert.</p>
            </section>
            <section class="sh-panel">
                <h3>Technische Einstellungen</h3>
                <table class="sh-technical-table">
                    <thead>
                        <tr><th>Key</th><th>Wert</th><th>Typ</th></tr>
                    </thead>
                    <tbody>
                        <tr v-for="row in payload.settings" :key="'setting-' + row.key">
                            <td>{{ row.key }}</td>
                            <td>{{ row.value_text }}</td>
                            <td>{{ row.value_type }}</td>
                        </tr>
                    </tbody>
                </table>
            </section>
            <section class="sh-panel">
                <h3>Räume</h3>
                <div class="sh-secondary-list">
                    <span class="sh-secondary-pill" v-for="room in payload.rooms" :key="'config-room-' + room.slug">
                        {{ room.name }}
                    </span>
                </div>
            </section>
            <section class="sh-panel">
                <h3>[SIM] Validierung</h3>
                <p class="sh-muted">Getrennter Technikpfad für sim_*. Kein Teil der Hauptnavigation.</p>
                <div class="sh-control-row">
                    <a class="sh-btn sh-btn-muted" href="/dashboard/simulation">[SIM] öffnen</a>
                </div>
            </section>
        </div>

        <section class="sh-panel">
            <h3>Server-Status JSON</h3>
            <pre class="sh-code-block">{{ payload.server_status_pretty || 'Kein server_status_json gespeichert.' }}</pre>
        </section>
    </div>
</template>

<script>
export default {
    data() {
        return {
            payload: { settings: [], rooms: [], weather: {}, devices: [] },
            selectedDeviceId: ""
        };
    },
    methods: {
        reload() {
            this.send({ payload: { view: "config", action: "load" } });
        }
    },
    watch: {
        msg: {
            immediate: true,
            handler(value) {
                const payload = value?.payload || {};
                if (payload.kind === "config") {
                    this.payload = payload;
                    if (!this.selectedDeviceId || !payload.devices.some((item) => item.device_id === this.selectedDeviceId)) {
                        this.selectedDeviceId = payload.devices.length ? payload.devices[0].device_id : "";
                    }
                }
            }
        }
    },
    mounted() {
        setTimeout(() => this.reload(), 120);
    }
}
</script>

${dashboardSharedStyle}
`;

const logsTemplate = `
<template>
    <div class="sh-page">
        <div class="sh-toolbar">
            <div class="sh-toolbar-actions">
                <button class="sh-btn sh-btn-muted" @click="reload">Neu laden</button>
                ${renderDashboardNavHtml()}
            </div>
        </div>

        <div class="sh-empty" v-if="!rows.length">Keine Audit-Einträge vorhanden.</div>
        <table class="sh-log-table" v-else>
            <thead>
                <tr>
                    <th>Zeit</th>
                    <th>Richtung</th>
                    <th>Scope</th>
                    <th>Kanal</th>
                    <th>Entity</th>
                    <th>Topic</th>
                    <th>Payload</th>
                </tr>
            </thead>
            <tbody>
                <tr v-for="row in rows" :key="'log-' + row.id">
                    <td>{{ row.occurred_at }}</td>
                    <td>{{ row.direction }}</td>
                    <td>{{ row.scope }}</td>
                    <td>{{ row.channel }}</td>
                    <td>{{ row.entity_label }}</td>
                    <td>{{ row.topic }}</td>
                    <td><pre class="sh-code-block">{{ row.payload_preview }}</pre></td>
                </tr>
            </tbody>
        </table>
    </div>
</template>

<script>
export default {
    computed: {
        rows() {
            return Array.isArray(this.msg?.payload?.rows) ? this.msg.payload.rows : [];
        }
    },
    methods: {
        reload() {
            this.send({ payload: { view: "logs", action: "load" } });
        }
    },
    mounted() {
        setTimeout(() => this.reload(), 120);
    }
}
</script>

${dashboardSharedStyle}
`;

const dashboardCommonHelperLines = [
    'const parseJson = (value) => {',
    '    if (value === null || value === undefined || value === "") return null;',
    '    if (typeof value === "object") return value;',
    '    try {',
    '        return JSON.parse(value);',
    '    } catch (error) {',
    '        return null;',
    '    }',
    '};',
    'const normalizeString = (value) => typeof value === "string" ? value.trim() : "";',
    'const normalizeBool = (value) => {',
    '    if (value === true || value === 1 || value === "1") return true;',
    '    if (value === false || value === 0 || value === "0") return false;',
    '    if (typeof value === "string") {',
    '        const lowered = value.trim().toLowerCase();',
    '        if (lowered === "true" || lowered === "yes" || lowered === "ja") return true;',
    '        if (lowered === "false" || lowered === "no" || lowered === "nein") return false;',
    '    }',
    '    return null;',
    '};',
    'const normalizeNumber = (value) => {',
    '    if (typeof value === "number" && Number.isFinite(value)) return value;',
    '    if (typeof value === "string" && value.trim()) {',
    '        const parsed = Number(value);',
    '        return Number.isFinite(parsed) ? parsed : null;',
    '    }',
    '    return null;',
    '};',
    'const slugify = (value) => normalizeString(value).toLowerCase().replace(/[^a-z0-9]+/g, "-").replace(/^-+|-+$/g, "");',
    'const dashboardTimeZone = normalizeString(env.get("TZ")) || "Europe/Berlin";',
    'const makeLocalDateFormatter = (options) => new Intl.DateTimeFormat("de-DE", Object.assign({ timeZone: dashboardTimeZone }, options));',
    'const parseTimestamp = (value) => {',
    '    const text = normalizeString(value);',
    '    if (!text) return null;',
    '    const tsMs = Date.parse(text);',
    '    return Number.isFinite(tsMs) ? tsMs : null;',
    '};',
    'const formatRelativeUnit = (count, singular, plural) => "vor " + String(count) + " " + (count === 1 ? singular : plural);',
    'const formatLocalTimestamp = (value, options = {}) => {',
    '    const tsMs = parseTimestamp(value);',
    '    if (tsMs === null) return normalizeString(value) || "-";',
    '    const date = new Date(tsMs);',
    '    if (Number.isNaN(date.getTime())) return normalizeString(value) || "-";',
    '    const includeSeconds = options.includeSeconds !== false;',
    '    const includeYear = options.includeYear !== false;',
    '    return makeLocalDateFormatter({',
    '        day: "2-digit",',
    '        month: "2-digit",',
    '        ...(includeYear ? { year: "numeric" } : {}),',
    '        hour: "2-digit",',
    '        minute: "2-digit",',
    '        ...(includeSeconds ? { second: "2-digit" } : {})',
    '    }).format(date);',
    '};',
    'const formatRelativeSeenLabel = (value) => {',
    '    const tsMs = parseTimestamp(value);',
    '    if (tsMs === null) return normalizeString(value) || "-";',
    '    const ageMs = Date.now() - tsMs;',
    '    if (!Number.isFinite(ageMs) || ageMs < 0) return formatLocalTimestamp(value, { includeSeconds: false });',
    '    const ageSeconds = Math.max(1, Math.floor(ageMs / 1000));',
    '    if (ageSeconds < 60) return formatRelativeUnit(ageSeconds, "Sekunde", "Sekunden");',
    '    const ageMinutes = Math.floor(ageSeconds / 60);',
    '    if (ageMinutes < 60) return formatRelativeUnit(ageMinutes, "Minute", "Minuten");',
    '    const ageHours = Math.floor(ageMinutes / 60);',
    '    if (ageHours < 24) return formatRelativeUnit(ageHours, "Stunde", "Stunden");',
    '    return formatLocalTimestamp(value, { includeSeconds: false });',
    '};',
    'const metaKeys = new Set(["source", "sim_case", "master_id", "node_id", "device_id", "device_type", "device_class", "power_source", "room", "room_slug", "ts", "timestamp"]);',
    'const labelMap = {',
    '    relay_1: "Relais 1",',
    '    relay_2: "Relais 2",',
    '    temp_01c: "Temperatur",',
    '    temperature_c: "Temperatur",',
    '    hum_01pct: "Luftfeuchte",',
    '    humidity_pct: "Luftfeuchte",',
    '    lux: "Helligkeit",',
    '    battery_mv: "Batteriespannung",',
    '    battery_pct: "Batteriestand",',
    '    motion: "Bewegung",',
    '    contact: "Kontakt",',
    '    online: "Online",',
    '    fault: "Störung",',
    '    cover_mode: "Rollladenmodus",',
    '    cover_state: "Fahrzustand",',
    '    cover_position: "Position",',
    '    cover_calibrated: "Kalibriert"',
    '};',
    'const coverStateLabel = (value) => {',
    '    if (value === 1) return "fährt auf";',
    '    if (value === 2) return "fährt ab";',
    '    return "gestoppt";',
    '};',
    'const defaultRelayLabel = (key) => key === "relay_2" ? "Relais 2" : "Relais 1";',
    'const resolveRelayLabel = (key, relayLabels = {}) => normalizeString(relayLabels[key]) || defaultRelayLabel(key);',
    'const humanizeKey = (key) => labelMap[key] || String(key || "")',
    '    .replace(/_/g, " ")',
    '    .replace(/\\b\\w/g, (match) => match.toUpperCase());',
    'const normalizeMetricKey = (value) => normalizeString(value)',
    '    .toLowerCase()',
    '    .replace(/[^a-z0-9_]+/g, "_")',
    '    .replace(/^_+|_+$/g, "");',
    'const inferMetricUnit = (metric, explicitUnit = "") => {',
    '    const unit = normalizeString(explicitUnit).toLowerCase();',
    '    if (unit) return unit;',
    '    if (/_01c$/i.test(metric)) return "0.1c";',
    '    if (/_01pct$/i.test(metric)) return "0.1pct";',
    '    if (/temp|_c$/i.test(metric)) return "c";',
    '    if (/hum|humid|_pct$|pct$/i.test(metric)) return "pct";',
    '    if (/eco2|co2/i.test(metric)) return "ppm";',
    '    if (/tvoc|voc/i.test(metric)) return "ppb";',
    '    if (/lux/i.test(metric)) return "lux";',
    '    if (/battery_pct/i.test(metric)) return "pct";',
    '    if (/battery_mv|_mv$/i.test(metric)) return "mv";',
    '    if (/battery_v|_v$|volt/i.test(metric)) return "v";',
    '    return "";',
    '};',
    'const formatMetricUnitLabel = (unit, metric) => {',
    '    const normalizedUnit = inferMetricUnit(metric, unit);',
    '    if (normalizedUnit === "0.1c" || normalizedUnit === "c") return "°C";',
    '    if (normalizedUnit === "0.1pct" || normalizedUnit === "pct") return "%";',
    '    if (normalizedUnit === "mv") return "mV";',
    '    if (normalizedUnit === "v") return "V";',
    '    if (normalizedUnit === "lux") return "lx";',
    '    if (normalizedUnit === "ppm" || normalizedUnit === "ppb") return normalizedUnit;',
    '    return normalizedUnit || "";',
    '};',
    'const formatChartRangeLabel = (rangeKey) => ({',
    '    "1h": "1 h",',
    '    "6h": "6 h",',
    '    "24h": "24 h",',
    '    "7d": "7 Tage",',
    '    "30d": "30 Tage"',
    '})[rangeKey] || String(rangeKey || "-");',
    'const formatChartStepLabel = (stepKey) => {',
    '    if (!stepKey) return "-";',
    '    return stepKey === "raw" ? "Rohdaten" : "Mittelwert je " + stepKey;',
    '};',
    'const formatChartMetricSummary = (metrics) => {',
    '    if (!Array.isArray(metrics) || !metrics.length) return "";',
    '    return metrics',
    '        .map((metric) => {',
    '            const normalizedMetric = normalizeMetricKey(metric);',
    '            if (!normalizedMetric) return "";',
    '            const baseLabel = humanizeKey(normalizedMetric);',
    '            const unitLabel = formatMetricUnitLabel("", normalizedMetric);',
    '            return unitLabel ? baseLabel + " [" + unitLabel + "]" : baseLabel;',
    '        })',
    '        .filter(Boolean)',
    '        .join(", ");',
    '};',
    'const makeChartDateFormatter = (options) => makeLocalDateFormatter(options);',
    'const formatChartTimeLabel = (tsMs, minTs, maxTs, variant = "tick") => {',
    '    const tsValue = normalizeNumber(tsMs);',
    '    if (tsValue === null) return "-";',
    '    const date = new Date(tsValue);',
    '    if (Number.isNaN(date.getTime())) return "-";',
    '    const minValue = normalizeNumber(minTs);',
    '    const maxValue = normalizeNumber(maxTs);',
    '    const spanMs = minValue !== null && maxValue !== null ? Math.max(0, maxValue - minValue) : 0;',
    '    if (variant === "point") return makeChartDateFormatter({ day: "2-digit", month: "2-digit", hour: "2-digit", minute: "2-digit", second: "2-digit" }).format(date);',
    '    if (variant === "range") return makeChartDateFormatter({ day: "2-digit", month: "2-digit", hour: "2-digit", minute: "2-digit" }).format(date);',
    '    if (spanMs <= 2 * 60 * 60 * 1000) return makeChartDateFormatter({ hour: "2-digit", minute: "2-digit" }).format(date);',
    '    if (spanMs <= 36 * 60 * 60 * 1000) return makeChartDateFormatter({ day: "2-digit", month: "2-digit", hour: "2-digit", minute: "2-digit" }).format(date);',
    '    if (spanMs <= 14 * 24 * 60 * 60 * 1000) return makeChartDateFormatter({ day: "2-digit", month: "2-digit", hour: "2-digit" }).format(date);',
    '    return makeChartDateFormatter({ day: "2-digit", month: "2-digit" }).format(date);',
    '};',
    'const niceNumber = (range, shouldRound) => {',
    '    if (!Number.isFinite(range) || range <= 0) return 1;',
    '    const exponent = Math.floor(Math.log10(range));',
    '    const fraction = range / Math.pow(10, exponent);',
    '    let niceFraction;',
    '    if (shouldRound) {',
    '        if (fraction < 1.5) niceFraction = 1;',
    '        else if (fraction < 3) niceFraction = 2;',
    '        else if (fraction < 7) niceFraction = 5;',
    '        else niceFraction = 10;',
    '    } else {',
    '        if (fraction <= 1) niceFraction = 1;',
    '        else if (fraction <= 2) niceFraction = 2;',
    '        else if (fraction <= 5) niceFraction = 5;',
    '        else niceFraction = 10;',
    '    }',
    '    return niceFraction * Math.pow(10, exponent);',
    '};',
    'const buildNiceScale = (minValue, maxValue, tickCount = 4) => {',
    '    const lower = normalizeNumber(minValue);',
    '    const upper = normalizeNumber(maxValue);',
    '    if (lower === null || upper === null) {',
    '        return { min: 0, max: 1, ticks: [0, 0.5, 1] };',
    '    }',
    '    if (lower === upper) {',
    '        const pad = Math.max(Math.abs(lower) * 0.02, 1);',
    '        return buildNiceScale(lower - pad, upper + pad, tickCount);',
    '    }',
    '    const boundedMin = Math.min(lower, upper);',
    '    const boundedMax = Math.max(lower, upper);',
    '    const range = niceNumber(boundedMax - boundedMin, false);',
    '    const step = niceNumber(range / Math.max(tickCount - 1, 1), true);',
    '    const niceMin = Math.floor(boundedMin / step) * step;',
    '    const niceMax = Math.ceil(boundedMax / step) * step;',
    '    const ticks = [];',
    '    for (let value = niceMin; value <= niceMax + step * 0.5; value += step) {',
    '        ticks.push(Number(value.toFixed(6)));',
    '        if (ticks.length >= 8) break;',
    '    }',
    '    return {',
    '        min: niceMin,',
    '        max: niceMax === niceMin ? niceMin + step : niceMax,',
    '        ticks: ticks.length ? ticks : [niceMin, niceMax]',
    '    };',
    '};',
    'const defaultChartPadding = (unit) => {',
    '    const normalizedUnit = inferMetricUnit(unit, unit);',
    '    if (normalizedUnit === "pct" || normalizedUnit === "0.1pct") return 2;',
    '    if (normalizedUnit === "mv") return 5;',
    '    if (normalizedUnit === "v") return 0.1;',
    '    if (normalizedUnit === "c" || normalizedUnit === "0.1c") return 0.5;',
    '    if (normalizedUnit === "lux") return 10;',
    '    if (normalizedUnit === "ppm" || normalizedUnit === "ppb") return 20;',
    '    return 1;',
    '};',
    'const buildChartXTicks = (minTs, maxTs) => {',
    '    const minValue = normalizeNumber(minTs);',
    '    const maxValue = normalizeNumber(maxTs);',
    '    if (minValue === null || maxValue === null) return [];',
    '    const plotWidth = chartGeometry.right - chartGeometry.left;',
    '    if (minValue === maxValue) {',
    '        return [{',
    '            x: Number((chartGeometry.left + plotWidth / 2).toFixed(1)),',
    '            ts_ms: minValue,',
    '            label: formatChartTimeLabel(minValue, minValue, maxValue, "tick")',
    '        }];',
    '    }',
    '    const spanMs = Math.max(0, maxValue - minValue);',
    '    const tickCount = spanMs <= 2 * 60 * 60 * 1000 ? 4 : spanMs <= 24 * 60 * 60 * 1000 ? 5 : 4;',
    '    const ticks = [];',
    '    for (let index = 0; index < tickCount; index += 1) {',
    '        const ratio = tickCount === 1 ? 0 : index / (tickCount - 1);',
    '        const tsValue = minValue + spanMs * ratio;',
    '        ticks.push({',
    '            x: Number((chartGeometry.left + plotWidth * ratio).toFixed(1)),',
    '            ts_ms: Math.round(tsValue),',
    '            label: formatChartTimeLabel(tsValue, minValue, maxValue, "tick")',
    '        });',
    '    }',
    '    return ticks;',
    '};',
    'const buildPanelAxisLabel = (seriesList, unitKey) => {',
    '    const unitLabel = formatMetricUnitLabel(unitKey, unitKey);',
    '    if (Array.isArray(seriesList) && seriesList.length === 1) {',
    '        return unitLabel ? seriesList[0].label + " [" + unitLabel + "]" : seriesList[0].label;',
    '    }',
    '    return unitLabel ? "Messwert [" + unitLabel + "]" : "Messwert";',
    '};',
    'const isChartableMetric = (metric) => {',
    '    const normalizedMetric = normalizeMetricKey(metric);',
    '    if (!normalizedMetric) return false;',
    '    if (/relay|switch|cover|output|fault|online|status|event|command|motion|contact|window/i.test(normalizedMetric)) return false;',
    '    return /(temp|hum|humid|co2|eco2|tvoc|voc|lux|battery(_mv|_v|_pct)?|volt|pressure|current|power|energy|aqi|rain|water|flow|pm\\d*|soil|moist|uv)/i.test(normalizedMetric);',
    '};',
    'const finalizeChartMetricOptions = (items) => {',
    '    const deduped = new Map();',
    '    for (const item of items) {',
    '        const normalizedKey = normalizeMetricKey(item && item.key);',
    '        if (!normalizedKey || !isChartableMetric(normalizedKey)) continue;',
    '        const current = deduped.get(normalizedKey);',
    '        const label = normalizeString(item && item.label) || humanizeKey(normalizedKey);',
    '        const unit = inferMetricUnit(normalizedKey, item && item.unit);',
    '        if (!current) {',
    '            deduped.set(normalizedKey, { key: normalizedKey, label, unit });',
    '            continue;',
    '        }',
    '        if (!current.unit && unit) current.unit = unit;',
    '        if (!current.label && label) current.label = label;',
    '    }',
    '    const baseItems = Array.from(deduped.values());',
    '    const labelCounts = new Map();',
    '    for (const item of baseItems) {',
    '        const baseLabel = item.label || humanizeKey(item.key);',
    '        labelCounts.set(baseLabel, (labelCounts.get(baseLabel) || 0) + 1);',
    '    }',
    '    return baseItems',
    '        .map((item) => {',
    '            const baseLabel = item.label || humanizeKey(item.key);',
    '            const unitLabel = formatMetricUnitLabel(item.unit, item.key);',
    '            const duplicateLabel = (labelCounts.get(baseLabel) || 0) > 1;',
    '            return {',
    '                key: item.key,',
    '                label: duplicateLabel',
    '                    ? unitLabel',
    '                        ? baseLabel + " [" + unitLabel + "]"',
    '                        : baseLabel + " [" + item.key + "]"',
    '                    : baseLabel,',
    '                unit: item.unit || null',
    '            };',
    '        })',
    '        .sort((left, right) => left.label.localeCompare(right.label));',
    '};',
    'const isRollladenDevice = (flatState, meta, deviceInfo = {}) => {',
    '    const coverMode = normalizeBool(flatState && flatState.cover_mode);',
    '    if (coverMode === true) return true;',
    '    if (coverMode === false) return false;',
    '    const hints = [',
    '        normalizeString(deviceInfo.device_class),',
    '        normalizeString(deviceInfo.hardware_type),',
    '        normalizeString(meta && meta.device_class),',
    '        normalizeString(meta && meta.hardware_type),',
    '        normalizeString(meta && meta.device_type),',
    '        normalizeString(meta && meta.device_mode),',
    '        normalizeString(meta && meta.kind)',
    '    ].join(" ").toLowerCase();',
    '    return /\\brollladen\\b|\\brolladen\\b|\\bshutter\\b/.test(hints);',
    '};',
    'const clampPercent = (value) => {',
    '    const numberValue = normalizeNumber(value);',
    '    if (numberValue === null) return null;',
    '    return Math.max(0, Math.min(100, Math.round(numberValue)));',
    '};',
    'const isMasterDevice = (deviceInfo = {}) => {',
    '    const deviceRole = normalizeString(deviceInfo.device_role).toLowerCase();',
    '    const deviceClass = normalizeString(deviceInfo.device_class).toLowerCase();',
    '    return deviceRole === "master" || /master/.test(deviceClass);',
    '};',
    'const classifyDeviceVisual = (deviceInfo = {}, options = {}) => {',
    '    const controlKind = normalizeString(options.controlKind);',
    '    const relayCount = normalizeNumber(options.relayCount) || 0;',
    '    const sensorCount = normalizeNumber(options.sensorCount) || 0;',
    '    const deviceClass = normalizeString(deviceInfo.device_class).toLowerCase();',
    '    const hardwareType = normalizeString(deviceInfo.hardware_type).toLowerCase();',
    '    const powerSource = normalizeString(deviceInfo.power_source).toLowerCase();',
    '    if (controlKind === "cover") return { kind_label: "Rollladen", type_icon: "mdi-window-shutter", surface_class: "is-cover-card" };',
    '    if (controlKind === "relay") return { kind_label: relayCount > 1 ? "Relais 2-fach" : "Relais", type_icon: relayCount > 1 ? "mdi-lightbulb-group-outline" : "mdi-lightbulb-outline", surface_class: "is-relay-card" };',
    '    if (isMasterDevice(deviceInfo)) return { kind_label: "Bridge", type_icon: "mdi-router-wireless", surface_class: "is-master-card" };',
    '    if (sensorCount > 0 && (powerSource === "battery" || /bat_sen/.test(deviceClass) || /battery/.test(hardwareType))) return { kind_label: "Batteriesensor", type_icon: "mdi-home-battery-outline", surface_class: "is-battery-card" };',
    '    if (sensorCount > 0) return { kind_label: "Sensor", type_icon: "mdi-thermometer-lines", surface_class: "is-sensor-card" };',
    '    if (powerSource === "battery") return { kind_label: "Batteriegerät", type_icon: "mdi-battery-outline", surface_class: "is-battery-card" };',
    '    return { kind_label: "Gerät", type_icon: "mdi-chip", surface_class: "" };',
    '};',
    'const buildRelayControls = (relayKeys, flatState, relayLabels = {}) => {',
    '    const relays = relayKeys.map((key) => {',
    '        const active = normalizeBool(flatState[key]) === true;',
    '        const label = resolveRelayLabel(key, relayLabels);',
    '        return {',
    '            key,',
    '            label,',
    '            active,',
    '            state_label: active ? "Ein" : "Aus",',
    '            action_label: active ? "Tippen zum Ausschalten" : "Tippen zum Einschalten",',
    '            icon: active ? "mdi-lightbulb-on" : "mdi-lightbulb-outline"',
    '        };',
    '    });',
    '    const activeCount = relays.filter((relay) => relay.active).length;',
    '    return {',
    '        kind: "relay",',
    '        relays,',
    '        active_count: activeCount',
    '    };',
    '};',
    'const buildCoverControls = (flatState, coverCalibrated) => {',
    '    const positionNumber = clampPercent(flatState.cover_position);',
    '    const stateNumber = normalizeNumber(flatState.cover_state);',
    '    const movingUp = stateNumber === 1;',
    '    const movingDown = stateNumber === 2;',
    '    const positionKnown = positionNumber !== null;',
    '    return {',
    '        kind: "cover",',
    '        position_value: positionKnown ? positionNumber : 0,',
    '        position_text: positionKnown ? String(positionNumber) + " %" : "Position unbekannt",',
    '        position_known: positionKnown,',
    '        state_text: coverStateLabel(stateNumber),',
    '        state_icon: movingUp ? "mdi-arrow-up" : movingDown ? "mdi-arrow-down" : "mdi-stop",',
    '        motion_class: movingUp ? "is-moving-up" : movingDown ? "is-moving-down" : "is-resting",',
    '        shutter_style: positionKnown ? "height:" + String(positionNumber) + "%;" : "",',
    '        slider_enabled: false,',
    '        calibration_label: coverCalibrated === true ? "kalibriert" : "nicht kalibriert",',
    '        slider_note: coverCalibrated === true ? "Zielposition ist über die aktuelle MQTT-Bridge noch nicht verdrahtet." : "Slider gesperrt: Rollladen ist nicht kalibriert oder die Kalibrierung wurde noch nicht gemeldet."',
    '    };',
    '};',
    'const filterPrimaryValues = (rows, options = {}) => rows.filter((row) => {',
    '    const key = row && row.key ? row.key : "";',
    '    if (key === "cover") return false;',
    '    if (options.controlKind !== "cover" && key === "cover_position") return false;',
    '    return true;',
    '});',
    'const isNegativeStateNoiseKey = (key) => /(fault|alarm|error|warn|disturb|stoer|stoe)/i.test(normalizeMetricKey(key));',
    'const filterSecondaryValues = (rows, options = {}) => rows.filter((row) => {',
    '    const key = row && row.key ? row.key : "";',
    '    const boolValue = normalizeBool(row && row.raw_value);',
    '    if (key === "cover") return false;',
    '    if (boolValue === false && isNegativeStateNoiseKey(key)) return false;',
    '    if (options.controlKind === "cover") {',
    '        return !["relay_1", "relay_2", "cover_mode", "cover_position", "cover_calibrated"].includes(key);',
    '    }',
    '    if (options.controlKind === "relay" && ["relay_1", "relay_2"].includes(key)) return false;',
    '    return !["cover_mode", "cover_state", "cover_position", "cover_calibrated"].includes(key);',
    '});',
    'const formatValueText = (key, value, unit, role) => {',
    '    if (/cover_state/i.test(key)) {',
    '        const numberValue = normalizeNumber(value);',
    '        return numberValue === null ? "-" : coverStateLabel(numberValue);',
    '    }',
    '    const numericMetric = role === "sensor" || isChartableMetric(normalizeMetricKey(key)) || /position/i.test(key);',
    '    const numberValue = normalizeNumber(value);',
    '    if (numericMetric && numberValue !== null) {',
    '        unit = inferMetricUnit(key, unit);',
    '        if (unit === "0.1c") return (numberValue / 10).toFixed(1) + " °C";',
    '        if (unit === "0.1pct") return (numberValue / 10).toFixed(1) + " %";',
    '        if (unit === "pct" || /_pct$/i.test(key) || /position/i.test(key)) return String(Math.round(numberValue)) + " %";',
    '        if (unit === "c") return numberValue.toFixed(1) + " °C";',
    '        if (unit === "mv") return String(Math.round(numberValue)) + " mV";',
    '        if (unit === "lux") return String(Math.round(numberValue)) + " lx";',
    '        if (unit === "ppm" || unit === "ppb") return String(Math.round(numberValue)) + " " + unit;',
    '        return Number.isInteger(numberValue) ? String(numberValue) : numberValue.toFixed(1);',
    '    }',
    '    const boolValue = normalizeBool(value);',
    '    if (boolValue !== null) {',
    '        if (/online/i.test(key)) return boolValue ? "online" : "offline";',
    '        if (/fault/i.test(key)) return boolValue ? "ja" : "nein";',
    '        if (/relay|switch|light|output/i.test(key) || role === "actuator") return boolValue ? "ein" : "aus";',
    '        return boolValue ? "ja" : "nein";',
    '    }',
    '    if (numberValue !== null) {',
    '        unit = inferMetricUnit(key, unit);',
    '        if (unit === "0.1c") return (numberValue / 10).toFixed(1) + " °C";',
    '        if (unit === "0.1pct") return (numberValue / 10).toFixed(1) + " %";',
    '        if (unit === "pct" || /_pct$/i.test(key) || /position/i.test(key)) return String(Math.round(numberValue)) + " %";',
    '        if (unit === "c") return numberValue.toFixed(1) + " °C";',
    '        if (unit === "mv") return String(Math.round(numberValue)) + " mV";',
    '        if (unit === "lux") return String(Math.round(numberValue)) + " lx";',
    '        if (unit === "ppm" || unit === "ppb") return String(Math.round(numberValue)) + " " + unit;',
    '        return Number.isInteger(numberValue) ? String(numberValue) : numberValue.toFixed(1);',
    '    }',
    '    return value === null || value === undefined || value === "" ? "-" : String(value);',
    '};',
    'const isoLabel = (value) => formatLocalTimestamp(value);',
    'const colorPalette = ["#1d4ed8", "#0f766e", "#b45309", "#be123c", "#6d28d9", "#0f172a"];',
    'const chartGeometry = { width: 640, height: 280, left: 72, right: 618, top: 28, bottom: 212 };',
    'const flattenState = (payload) => {',
    '    const flat = {};',
    '    const visit = (value, prefix) => {',
    '        if (value === null || value === undefined) return;',
    '        if (typeof value === "number" || typeof value === "string" || typeof value === "boolean") {',
    '            flat[prefix] = value;',
    '            return;',
    '        }',
    '        if (Array.isArray(value) || typeof value !== "object") return;',
    '        for (const [key, current] of Object.entries(value)) {',
    '            const nextPrefix = prefix ? prefix + "_" + key : key;',
    '            visit(current, nextPrefix);',
    '        }',
    '    };',
    '    if (payload && payload.sensors && typeof payload.sensors === "object" && !Array.isArray(payload.sensors)) {',
    '        visit(payload.sensors, "");',
    '    }',
    '    if (payload && typeof payload === "object") {',
    '        for (const [key, current] of Object.entries(payload)) {',
    '            if (metaKeys.has(key) || key === "sensors") continue;',
    '            if (typeof current === "number" || typeof current === "string" || typeof current === "boolean") {',
    '                flat[key] = current;',
    '            }',
    '        }',
    '    }',
    '    return flat;',
    '};',
    'const sortValues = (items) => items.sort((left, right) => left.label.localeCompare(right.label));',
    'const buildChartState = (status, message, extra = {}) => Object.assign({ status, message }, extra);',
    'const buildChartSeries = (rows, chartTitle, rangeLabel, stepLabel, deviceId, metricSummary = "") => {',
    '    const grouped = new Map();',
    '    for (const row of rows) {',
    '        const metric = normalizeMetricKey(row.metric);',
    '        const valueNum = normalizeNumber(row.value_num);',
    '        const tsText = normalizeString(row.ts);',
    '        const tsMs = Date.parse(tsText);',
    '        if (!metric || valueNum === null || !tsText || Number.isNaN(tsMs)) continue;',
    '        if (!grouped.has(metric)) {',
    '            grouped.set(metric, { metric, label: humanizeKey(metric), unit: inferMetricUnit(metric, row.unit), points: [] });',
    '        }',
    '        grouped.get(metric).points.push({ ts_ms: tsMs, raw_value: valueNum });',
    '    }',
    '    if (!rows.length) {',
    '        return buildChartState("no_data", "Keine Influx-Daten für die aktuelle Auswahl.", { title: chartTitle, range_label: rangeLabel, step_label: stepLabel, device_id: deviceId || "", metric_summary: metricSummary });',
    '    }',
    '    const series = Array.from(grouped.values()).filter((item) => item.points.length);',
    '    if (!series.length) {',
    '        return buildChartState("render_error", "Influx lieferte Daten, aber keine auswertbaren Diagrammpunkte.", { title: chartTitle, range_label: rangeLabel, step_label: stepLabel, device_id: deviceId || "", metric_summary: metricSummary });',
    '    }',
    '    const allPoints = series.flatMap((item) => item.points);',
    '    const minTs = Math.min(...allPoints.map((point) => point.ts_ms));',
    '    const maxTs = Math.max(...allPoints.map((point) => point.ts_ms));',
    '    const tsRange = maxTs === minTs ? 1 : maxTs - minTs;',
    '    const plotWidth = chartGeometry.right - chartGeometry.left;',
    '    const plotHeight = chartGeometry.bottom - chartGeometry.top;',
    '    const panelsByUnit = new Map();',
    '    for (const item of series) {',
    '        const unitKey = inferMetricUnit(item.metric, item.unit) || "unitless";',
    '        if (!panelsByUnit.has(unitKey)) {',
    '            panelsByUnit.set(unitKey, { unit_key: unitKey, unit_label: formatMetricUnitLabel(unitKey, item.metric) || "ohne Einheit", series: [] });',
    '        }',
    '        panelsByUnit.get(unitKey).series.push(item);',
    '    }',
    '    const panels = Array.from(panelsByUnit.values()).map((panel, panelIndex) => {',
    '        const panelPoints = panel.series.flatMap((item) => item.points);',
    '        const minValue = Math.min(...panelPoints.map((point) => point.raw_value));',
    '        const maxValue = Math.max(...panelPoints.map((point) => point.raw_value));',
    '        const unitPadding = defaultChartPadding(panel.unit_key);',
    '        const valuePadding = maxValue === minValue',
    '            ? unitPadding',
    '            : Math.max((maxValue - minValue) * 0.08, Math.max(unitPadding * 0.25, 0.25));',
    '        const niceScale = buildNiceScale(minValue - valuePadding, maxValue + valuePadding, 4);',
    '        const scaleMin = niceScale.min;',
    '        const scaleMax = niceScale.max;',
    '        const valueRange = scaleMax - scaleMin || 1;',
    '        const panelSeries = panel.series',
    '            .sort((left, right) => left.label.localeCompare(right.label))',
    '            .map((item, index) => ({',
    '                metric: item.metric,',
    '                label: item.label,',
    '                unit: item.unit || null,',
    '                color: colorPalette[(panelIndex + index) % colorPalette.length],',
    '                point_count: item.points.length,',
    '                has_drawable_line: item.points.length > 1,',
    '                last_value_text: formatValueText(item.metric, item.points[item.points.length - 1].raw_value, item.unit, "sensor"),',
    '                points: item.points',
    '                    .sort((left, right) => left.ts_ms - right.ts_ms)',
    '                    .map((point) => ({',
    '                        x: minTs === maxTs ? Number((chartGeometry.left + plotWidth / 2).toFixed(1)) : Number((chartGeometry.left + ((point.ts_ms - minTs) / tsRange) * plotWidth).toFixed(1)),',
    '                        y: Number((chartGeometry.bottom - ((point.raw_value - scaleMin) / valueRange) * plotHeight).toFixed(1)),',
    '                        value_text: formatValueText(item.metric, point.raw_value, item.unit, "sensor"),',
    '                        value_num: point.raw_value,',
    '                        ts_ms: point.ts_ms',
    '                    }))',
    '            }));',
    '        const yTicks = niceScale.ticks.map((tickValue) => ({',
    '            value_num: tickValue,',
    '            label: formatValueText(panel.unit_key, tickValue, panel.unit_key, "sensor"),',
    '            y: Number((chartGeometry.bottom - ((tickValue - scaleMin) / valueRange) * plotHeight).toFixed(1))',
    '        }));',
    '        return {',
    '            unit_key: panel.unit_key,',
    '            unit_label: panel.unit_label,',
    '            axis_label: buildPanelAxisLabel(panelSeries, panel.unit_key),',
    '            metric_summary: panelSeries.map((item) => item.label).join(", "),',
    '            point_count: panelPoints.length,',
    '            min_label: formatValueText(panel.unit_key, minValue, panel.unit_key, "sensor"),',
    '            max_label: formatValueText(panel.unit_key, maxValue, panel.unit_key, "sensor"),',
    '            y_ticks: yTicks,',
    '            series: panelSeries',
    '        };',
    '    });',
    '    const hasDrawableLine = panels.some((panel) => panel.series.some((item) => item.has_drawable_line));',
    '    const sparseSeriesCount = panels.reduce((count, panel) => count + panel.series.filter((item) => !item.has_drawable_line).length, 0);',
    '    const notices = [];',
    '    if (panels.length > 1) notices.push("Mehrere Einheiten werden getrennt dargestellt.");',
    '    if (!hasDrawableLine) notices.push("Im gewählten Raster gibt es nur Einzelpunkte. Für Linien kleinere Schrittweite oder Rohdaten wählen.");',
    '    else if (sparseSeriesCount > 0) notices.push("Einzelne Kurven liefern im gewählten Raster nur Einzelpunkte.");',
    '    return {',
    '        status: hasDrawableLine ? "ok" : "sparse",',
    '        title: chartTitle,',
    '        range_label: rangeLabel,',
    '        step_label: stepLabel,',
    '        metric_summary: metricSummary || formatChartMetricSummary(series.map((item) => item.metric)),',
    '        x_axis_label: "Zeitachse (Lokalzeit)",',
    '        start_label: formatChartTimeLabel(minTs, minTs, maxTs, "range"),',
    '        end_label: formatChartTimeLabel(maxTs, minTs, maxTs, "range"),',
    '        x_ticks: buildChartXTicks(minTs, maxTs),',
    '        device_id: deviceId || "",',
    '        panels,',
    '        notice: notices.join(" "),',
    '        panel_count: panels.length',
    '    };',
    '};'
];

const routeDeviceDatasetActionFunc = String.raw`
const request = msg.payload || {};
const action = typeof request.action === "string" ? request.action : "load";
const view = typeof request.view === "string" ? request.view : "overview";

if (action === "relay_set" || action === "cover_move") {
    msg.payload = request;
    msg.dashboardView = view;
    return [null, msg, null];
}

if (action === "chart") {
    msg.payload = request;
    msg.chartTarget = "charts";
    return [null, null, msg];
}

msg.dashboardView = view;
return [msg, null, null];
`;

const buildDeviceDatasetQueryFunc = String.raw`
msg.topic = [
    "SELECT",
    "    d.device_id,",
    "    d.device_role,",
    "    d.device_class,",
    "    d.default_name,",
    "    d.display_name,",
    "    d.origin_master_id,",
    "    d.power_source,",
    "    d.status,",
    "    d.hardware_type,",
    "    d.firmware_version,",
    "    d.protocol_version,",
    "    d.manufacturer,",
    "    d.model,",
    "    d.first_seen_at,",
    "    d.last_seen_at,",
    "    d.last_meta_at,",
    "    d.last_status_at,",
    "    d.last_state_at,",
    "    d.meta_json,",
    "    ls.status_json,",
    "    ls.state_json,",
    "    ls.event_json,",
    "    ls.ack_json,",
    "    r.slug AS room_slug,",
    "    r.name AS room_name,",
    "    dc.capability_key,",
    "    dc.capability_role,",
    "    dc.data_type,",
    "    dc.unit,",
    "    dc.writable,",
    "    dc.readable,",
    "    dc.channel AS capability_channel,",
    "    dc.meta_json AS capability_meta_json,",
    "    cfg_cover.config_value AS cover_calibrated,",
    "    cfg_name.config_value AS ui_display_name,",
    "    cfg_relay_1_name.config_value AS ui_relay_1_name,",
    "    cfg_relay_2_name.config_value AS ui_relay_2_name,",
    "    (SELECT provider FROM weather_settings WHERE id = 1) AS weather_provider,",
    "    (SELECT location_label FROM weather_settings WHERE id = 1) AS weather_location_label,",
    "    (SELECT latitude FROM weather_settings WHERE id = 1) AS weather_latitude,",
    "    (SELECT longitude FROM weather_settings WHERE id = 1) AS weather_longitude,",
    "    (SELECT poll_interval_minutes FROM weather_settings WHERE id = 1) AS weather_poll_interval_minutes,",
    "    (SELECT enabled FROM weather_settings WHERE id = 1) AS weather_enabled,",
    "    (SELECT units FROM weather_settings WHERE id = 1) AS weather_units,",
    "    (SELECT updated_at FROM weather_settings WHERE id = 1) AS weather_updated_at",
    "FROM devices AS d",
    "LEFT JOIN device_last_state AS ls ON ls.device_id = d.device_id",
    "LEFT JOIN rooms AS r ON r.id = d.room_id",
    "LEFT JOIN device_capabilities AS dc ON dc.device_id = d.device_id",
    "LEFT JOIN device_config AS cfg_cover ON cfg_cover.device_id = d.device_id AND cfg_cover.config_key = 'cover_calibrated'",
    "LEFT JOIN device_config AS cfg_name ON cfg_name.device_id = d.device_id AND cfg_name.config_key = 'ui_display_name'",
    "LEFT JOIN device_config AS cfg_relay_1_name ON cfg_relay_1_name.device_id = d.device_id AND cfg_relay_1_name.config_key = 'ui_relay_1_name'",
    "LEFT JOIN device_config AS cfg_relay_2_name ON cfg_relay_2_name.device_id = d.device_id AND cfg_relay_2_name.config_key = 'ui_relay_2_name'",
    "ORDER BY",
    "    CASE WHEN d.device_role = 'master' THEN 0 ELSE 1 END,",
    "    lower(COALESCE(r.name, '')),",
    "    lower(COALESCE(cfg_name.config_value, d.display_name, d.device_id)),",
    "    lower(COALESCE(dc.capability_key, ''));"
].join(" ");

return msg;
`;

const buildDeviceDatasetPayloadFunc = String.raw`
${dashboardCommonHelperLines.join("\n")}
const rows = Array.isArray(msg.payload) ? msg.payload : [];
const byDevice = new Map();

for (const row of rows) {
    if (!row.device_id) {
        continue;
    }
    if (!byDevice.has(row.device_id)) {
        const meta = parseJson(row.meta_json) || {};
        const status = parseJson(row.status_json) || {};
        const state = parseJson(row.state_json) || {};
        const flatState = flattenState(state);
        const uiDisplayName = normalizeString(row.ui_display_name);
        const displayName = uiDisplayName || normalizeString(row.display_name) || row.device_id;
        const onlineBool = normalizeBool(status.online);
        const onlineLabel = onlineBool === true ? "online" : onlineBool === false ? "offline" : "unbekannt";
        const onlineClass = onlineBool === true ? "sh-chip-online" : onlineBool === false ? "sh-chip-offline" : "sh-chip-unknown";
        const roomName = normalizeString(row.room_name) || normalizeString(state.room) || normalizeString(meta.room) || "";
        const roomSlug = normalizeString(row.room_slug) || slugify(roomName);
        byDevice.set(row.device_id, {
            device_id: row.device_id,
            device_role: row.device_role || "node",
            device_class: row.device_class || "unknown",
            display_name: displayName,
            display_name_input: uiDisplayName || normalizeString(row.display_name) || "",
            default_name: row.default_name || row.device_id,
            origin_master_id: row.origin_master_id || "",
            power_source: row.power_source || "unknown",
            power_icon: row.power_source === "battery" ? "mdi-battery" : row.power_source === "mains" ? "mdi-power-plug" : "mdi-help-circle-outline",
            power_label: row.power_source === "battery" ? "BAT" : row.power_source === "mains" ? "AC" : "?",
            status: row.status || "",
            online_label: onlineLabel,
            online_class: onlineClass,
            room_name: roomName,
            room_slug: roomSlug,
            hardware_type: row.hardware_type || "",
            firmware_version: row.firmware_version || "",
            protocol_version: row.protocol_version || "",
            manufacturer: row.manufacturer || "",
            model: row.model || "",
            meta,
            status_json: status,
            state_json: state,
            flat_state: flatState,
            relay_name_config: {
                relay_1: normalizeString(row.ui_relay_1_name),
                relay_2: normalizeString(row.ui_relay_2_name)
            },
            simulation: row.device_id.startsWith("sim_") || meta.source === "simulation" || normalizeString(row.display_name).startsWith("[SIM]"),
            last_seen_at: row.last_seen_at || "",
            last_seen_label: formatRelativeSeenLabel(row.last_seen_at),
            last_seen_exact_label: isoLabel(row.last_seen_at),
            last_state_at: row.last_state_at || "",
            cover_calibrated: normalizeBool(row.cover_calibrated),
            capabilities: [],
            primary_values: [],
            secondary_values: [],
            sensor_metrics: [],
            chart_metrics: [],
            chart_unavailable_reason: "",
            controls: null,
            detail_url: "/dashboard/geraet?device=" + encodeURIComponent(row.device_id),
            info_rows: []
        });
    }
    const device = byDevice.get(row.device_id);
    if (row.capability_key) {
        device.capabilities.push({
            key: row.capability_key,
            role: row.capability_role || "unknown",
            type: row.data_type || "",
            unit: row.unit || "",
            writable: normalizeBool(row.writable) === true,
            readable: normalizeBool(row.readable) !== false,
            channel: row.capability_channel || "",
            meta: parseJson(row.capability_meta_json) || {}
        });
    }
}

const devices = Array.from(byDevice.values()).map((device) => {
    const state = device.state_json || {};
    const flatState = device.flat_state || {};
    const capabilities = device.capabilities.slice();
    const seenKeys = new Set();
    const sensorMetrics = [];
    const chartMetricCandidates = [];
    const primaryValues = [];
    const secondaryValues = [];

    for (const capability of capabilities) {
        const currentValue = flatState[capability.key];
        seenKeys.add(capability.key);
        const valueText = formatValueText(capability.key, currentValue, capability.unit, capability.role);
        const row = { key: capability.key, label: humanizeKey(capability.key), value_text: valueText, raw_value: currentValue };
        if (capability.role === "sensor") {
            primaryValues.push(row);
            sensorMetrics.push({ key: capability.key, label: humanizeKey(capability.key), unit: capability.unit || null });
            chartMetricCandidates.push({ key: capability.key, label: row.label, unit: capability.unit || null });
        } else {
            secondaryValues.push(row);
        }
    }

    for (const [key, value] of Object.entries(flatState)) {
        if (seenKeys.has(key)) {
            continue;
        }
        const valueText = formatValueText(key, value, null, "unknown");
        const row = { key, label: humanizeKey(key), value_text: valueText, raw_value: value };
        if (/temp|hum|lux|battery|co2|voc|rain|pressure|power|energy|position/i.test(key) && !/cover_state/i.test(key)) {
            primaryValues.push(row);
            if (!/cover_position/i.test(key)) {
                sensorMetrics.push({ key, label: humanizeKey(key), unit: null });
                chartMetricCandidates.push({ key, label: row.label, unit: null });
            }
        } else {
            secondaryValues.push(row);
        }
    }

    sortValues(primaryValues);
    sortValues(secondaryValues);
    const relayKeys = ["relay_1", "relay_2"].filter((key) => flatState[key] !== undefined || capabilities.some((item) => item.key === key));
    const isRollladen = isRollladenDevice(flatState, device.meta, { device_class: device.device_class, hardware_type: device.hardware_type });
    if (isRollladen && relayKeys.length >= 2) {
        device.controls = buildCoverControls(flatState, device.cover_calibrated === true);
    } else if (relayKeys.length) {
        device.controls = buildRelayControls(relayKeys, flatState, device.relay_name_config);
    }

    const controlKind = device.controls ? device.controls.kind : null;
    const visual = classifyDeviceVisual(device, { controlKind, relayCount: relayKeys.length, sensorCount: sensorMetrics.length });
    device.kind_label = visual.kind_label;
    device.type_icon = visual.type_icon;
    device.surface_class = visual.surface_class;
    device.primary_values = filterPrimaryValues(primaryValues, { controlKind }).slice(0, 6);
    device.highlight_values = device.primary_values.slice(0, controlKind === "cover" ? 1 : 3);
    device.secondary_values = filterSecondaryValues(secondaryValues, { controlKind }).slice(0, 6);
    device.sensor_metrics = sensorMetrics.filter((item, index, list) => list.findIndex((current) => current.key === item.key) === index);
    device.chart_metrics = finalizeChartMetricOptions(chartMetricCandidates);
    device.chart_unavailable_reason = device.sensor_metrics.length > 0 && !device.chart_metrics.length
        ? "Dieses Gerät meldet Sensorwerte, aber keine im aktuellen Influx-Schema chartbaren Zeitreihen."
        : "";
    device.summary_label = controlKind === "relay"
        ? ""
        : controlKind === "cover"
            ? device.controls.state_text
            : device.highlight_values.length === 0 && device.primary_values[0]
                ? device.primary_values[0].label + ": " + device.primary_values[0].value_text
                : "";
    device.info_rows = [
        { key: "device_class", label: "Klasse", value_text: device.device_class || "-" },
        { key: "origin_master_id", label: "Master", value_text: device.origin_master_id || "-" },
        { key: "firmware_version", label: "Firmware", value_text: device.firmware_version || "-" },
        { key: "protocol_version", label: "Protokoll", value_text: device.protocol_version || "-" },
        { key: "last_state_at", label: "Letzter state", value_text: isoLabel(device.last_state_at) }
    ];
    return device;
});

const overviewDevices = devices.filter((device) => !isMasterDevice(device));
const summary = {
    device_count: overviewDevices.length,
    master_count: devices.filter((device) => isMasterDevice(device)).length
};

const weather = rows.length ? {
    enabled: normalizeBool(rows[0].weather_enabled) === true,
    provider_label: normalizeString(rows[0].weather_provider) || "open-meteo",
    provider: normalizeString(rows[0].weather_provider) || "open-meteo",
    location_label: normalizeString(rows[0].weather_location_label),
    coords_label: normalizeNumber(rows[0].weather_latitude) !== null && normalizeNumber(rows[0].weather_longitude) !== null
        ? String(rows[0].weather_latitude) + ", " + String(rows[0].weather_longitude)
        : "-",
    poll_label: String(normalizeNumber(rows[0].weather_poll_interval_minutes) || 15) + " min",
    summary_line: normalizeBool(rows[0].weather_enabled) === true ? "Wetterkonfiguration vorhanden, aber noch ohne produktiven Live-Feed." : "Noch nicht aktiviert.",
    note: normalizeBool(rows[0].weather_enabled) === true
        ? "Die Homepage zeigt den aktuellen Wetter-Setup-Stand. Ein produktiver Wetterdatenpfad ist serverseitig noch offen."
        : "Wetter ist noch nicht aktiviert.",
    updated_at: isoLabel(rows[0].weather_updated_at)
} : null;

const roomMap = new Map();
for (const device of devices) {
    const key = device.room_slug || "ohne-raum";
    if (!roomMap.has(key)) {
        roomMap.set(key, {
            slug: key,
            name: device.room_name || "Ohne Raum",
            online_count: 0,
            offline_count: 0,
            devices: []
        });
    }
    const room = roomMap.get(key);
    room.devices.push({ device_id: device.device_id, display_name: device.display_name, detail_url: device.detail_url });
    if (device.online_label === "online") room.online_count += 1;
    if (device.online_label === "offline") room.offline_count += 1;
}

const view = msg.dashboardView || "overview";
let filteredDevices = view === "overview" ? overviewDevices : devices;
let page = { key: view, show_controls: true };
if (view === "devices") {
    page.title = "Geräte";
} else if (view === "sensors") {
    page.title = "Sensoren";
    filteredDevices = devices.filter((device) => device.sensor_metrics.length > 0);
} else if (view === "actors") {
    page.title = "Aktoren";
    filteredDevices = devices.filter((device) => device.controls && device.controls.kind === "relay");
} else if (view === "covers") {
    page.title = "Rollläden";
    filteredDevices = devices.filter((device) => device.controls && device.controls.kind === "cover");
} else if (view === "charts") {
    page.title = "Diagramme";
    page.show_controls = false;
    filteredDevices = devices.filter((device) => device.chart_metrics.length > 0);
} else {
    page.title = "Übersicht";
    filteredDevices = overviewDevices;
}

msg.payload = {
    page,
    summary,
    weather,
    devices: filteredDevices,
    rooms: Array.from(roomMap.values()).map((room) => ({
        slug: room.slug,
        name: room.name,
        device_count: room.devices.length,
        online_count: room.online_count,
        offline_count: room.offline_count,
        devices: room.devices
    }))
};

return msg;
`;

const routeDeviceDatasetPayloadFunc = String.raw`
const pageKey = msg.payload && msg.payload.page && msg.payload.page.key ? msg.payload.page.key : "overview";
return [
    pageKey === "overview" ? msg : null,
    pageKey === "rooms" ? msg : null,
    pageKey === "devices" ? msg : null,
    pageKey === "sensors" ? msg : null,
    pageKey === "actors" ? msg : null,
    pageKey === "covers" ? msg : null,
    pageKey === "charts" ? msg : null
];
`;

const buildGenericCommandFunc = String.raw`
const request = msg.payload || {};
const deviceId = typeof request.device_id === "string" ? request.device_id.trim() : "";
if (!deviceId) {
    return null;
}

const toCommandMessages = (commands) => commands.map((command, index) => {
    const occurredAt = new Date().toISOString();
    const requestId = [
        "nodered",
        request.view || "dashboard",
        deviceId,
        command.kind,
        index,
        Date.now()
    ].join("_");
    const payload = command.kind === "relay_set"
        ? { cmd: "set_relay", [command.relay_key]: command.state === true, request_id: requestId }
        : null;
    if (!payload) {
        return null;
    }
    const rawJson = JSON.stringify(payload);
    return {
        publish: {
            topic: "smarthome/node/" + deviceId + "/cmd/set",
            payload: rawJson
        },
        audit: {
            payload: {
                topic: "smarthome/node/" + deviceId + "/cmd/set",
                scope: "node",
                entityId: deviceId,
                channel: "cmd/set",
                direction: "egress",
                retain: false,
                ts: occurredAt,
                receivedAt: occurredAt,
                payload,
                rawJson
            }
        }
    };
}).filter(Boolean);

let commands = [];
if (request.action === "relay_set") {
    commands = [{
        kind: "relay_set",
        relay_key: request.relay_key === "relay_2" ? "relay_2" : "relay_1",
        state: request.state === true || request.state === "true" || request.state === 1 || request.state === "1"
    }];
} else if (request.action === "cover_move") {
    const direction = typeof request.direction === "string" ? request.direction : "";
    if (direction === "up") {
        commands = [{ kind: "relay_set", relay_key: "relay_1", state: true }];
    } else if (direction === "down") {
        commands = [{ kind: "relay_set", relay_key: "relay_2", state: true }];
    } else if (direction === "stop") {
        commands = [
            { kind: "relay_set", relay_key: "relay_1", state: false },
            { kind: "relay_set", relay_key: "relay_2", state: false }
        ];
    }
}

if (!commands.length) {
    return null;
}

const commandMessages = toCommandMessages(commands);
return [
    commandMessages.map((item) => item.publish),
    commandMessages.map((item) => item.audit)
];
`;

const routeDetailActionFunc = String.raw`
const request = msg.payload || {};
const action = typeof request.action === "string" ? request.action : "load";
msg.detailDeviceId = typeof request.device_id === "string" ? request.device_id.trim() : "";

if (action === "relay_set" || action === "cover_move") {
    msg.payload = Object.assign({}, request, { view: "detail" });
    return [null, null, null, null, msg];
}
if (action === "save") {
    return [null, msg, null, null, null];
}
if (action === "delete") {
    return [null, null, msg, null, null];
}
if (action === "chart") {
    msg.chartTarget = "detail";
    return [null, null, null, msg, null];
}
return [msg, null, null, null, null];
`;

const buildDeviceDetailQueryFunc = String.raw`
${toSqlLines.join("\n")}
const deviceId = typeof msg.detailDeviceId === "string" ? msg.detailDeviceId.trim() : "";
if (!deviceId) {
    msg.payload = { kind: "detail", device: null, room_options: [], sensor_metrics: [], chart_metrics: [], chart_unavailable_reason: "", technical_rows: [], config_rows: [] };
    return msg;
}

msg.topic = [
    "SELECT",
    "    'device' AS row_type,",
    "    d.device_id,",
    "    d.device_role,",
    "    d.device_class,",
    "    d.default_name,",
    "    d.display_name,",
    "    d.origin_master_id,",
    "    d.power_source,",
    "    d.status,",
    "    d.hardware_type,",
    "    d.firmware_version,",
    "    d.protocol_version,",
    "    d.manufacturer,",
    "    d.model,",
    "    d.first_seen_at,",
    "    d.last_seen_at,",
    "    d.last_meta_at,",
    "    d.last_status_at,",
    "    d.last_state_at,",
    "    d.meta_json,",
    "    ls.status_json,",
    "    ls.state_json,",
    "    ls.event_json,",
    "    ls.ack_json,",
    "    r.slug AS room_slug,",
    "    r.name AS room_name,",
    "    cfg_name.config_value AS ui_display_name,",
    "    cfg_relay_1_name.config_value AS ui_relay_1_name,",
    "    cfg_relay_2_name.config_value AS ui_relay_2_name,",
    "    cfg_cover.config_value AS cover_calibrated,",
    "    NULL AS capability_key,",
    "    NULL AS capability_role,",
    "    NULL AS data_type,",
    "    NULL AS unit,",
    "    NULL AS writable,",
    "    NULL AS readable,",
    "    NULL AS capability_meta_json,",
    "    NULL AS config_key,",
    "    NULL AS config_value,",
    "    NULL AS config_type,",
    "    NULL AS config_source,",
    "    NULL AS room_option_slug,",
    "    NULL AS room_option_name",
    "FROM devices AS d",
    "LEFT JOIN device_last_state AS ls ON ls.device_id = d.device_id",
    "LEFT JOIN rooms AS r ON r.id = d.room_id",
    "LEFT JOIN device_config AS cfg_name ON cfg_name.device_id = d.device_id AND cfg_name.config_key = 'ui_display_name'",
    "LEFT JOIN device_config AS cfg_relay_1_name ON cfg_relay_1_name.device_id = d.device_id AND cfg_relay_1_name.config_key = 'ui_relay_1_name'",
    "LEFT JOIN device_config AS cfg_relay_2_name ON cfg_relay_2_name.device_id = d.device_id AND cfg_relay_2_name.config_key = 'ui_relay_2_name'",
    "LEFT JOIN device_config AS cfg_cover ON cfg_cover.device_id = d.device_id AND cfg_cover.config_key = 'cover_calibrated'",
    "WHERE d.device_id = " + toSql(deviceId),
    "UNION ALL",
    "SELECT",
    "    'capability' AS row_type,",
    "    dc.device_id,",
    "    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,",
    "    NULL, NULL,",
    "    NULL,",
    "    NULL,",
    "    NULL,",
    "    NULL,",
    "    dc.capability_key,",
    "    dc.capability_role,",
    "    dc.data_type,",
    "    dc.unit,",
    "    dc.writable,",
    "    dc.readable,",
    "    dc.meta_json,",
    "    NULL, NULL, NULL, NULL, NULL, NULL",
    "FROM device_capabilities AS dc",
    "WHERE dc.device_id = " + toSql(deviceId),
    "UNION ALL",
    "SELECT",
    "    'config' AS row_type,",
    "    cfg.device_id,",
    "    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,",
    "    NULL, NULL,",
    "    NULL,",
    "    NULL,",
    "    NULL,",
    "    NULL,",
    "    NULL, NULL, NULL, NULL, NULL, NULL, NULL,",
    "    cfg.config_key,",
    "    cfg.config_value,",
    "    cfg.config_type,",
    "    cfg.source_channel,",
    "    NULL, NULL",
    "FROM device_config AS cfg",
    "WHERE cfg.device_id = " + toSql(deviceId),
    "UNION ALL",
    "SELECT",
    "    'room' AS row_type,",
    "    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,",
    "    NULL, NULL,",
    "    NULL,",
    "    NULL,",
    "    NULL,",
    "    NULL,",
    "    NULL, NULL, NULL, NULL, NULL, NULL, NULL,",
    "    NULL, NULL, NULL, NULL,",
    "    rooms.slug,",
    "    rooms.name",
    "FROM rooms",
    "ORDER BY row_type, room_option_name, capability_key, config_key;"
].join(" ");

return msg;
`;

const buildDeviceDetailPayloadFunc = String.raw`
${dashboardCommonHelperLines.join("\n")}
const rows = Array.isArray(msg.payload) ? msg.payload : [];
const result = {
    kind: "detail",
    device: null,
    sensor_metrics: [],
    chart_metrics: [],
    chart_unavailable_reason: "",
    technical_rows: [],
    config_rows: [],
    room_options: []
};

for (const row of rows) {
    if (row.row_type === "room" && row.room_option_slug) {
        result.room_options.push({ slug: row.room_option_slug, name: row.room_option_name || row.room_option_slug });
    }
}

const deviceRow = rows.find((row) => row.row_type === "device");
if (!deviceRow) {
    msg.payload = result;
    return msg;
}

const meta = parseJson(deviceRow.meta_json) || {};
const status = parseJson(deviceRow.status_json) || {};
const state = parseJson(deviceRow.state_json) || {};
const flatState = flattenState(state);
const uiDisplayName = normalizeString(deviceRow.ui_display_name);
const displayName = uiDisplayName || normalizeString(deviceRow.display_name) || deviceRow.device_id;
const onlineBool = normalizeBool(status.online);
const relayNameConfig = {
    relay_1: normalizeString(deviceRow.ui_relay_1_name),
    relay_2: normalizeString(deviceRow.ui_relay_2_name)
};
const capabilities = rows
    .filter((row) => row.row_type === "capability" && row.capability_key)
    .map((row) => ({
        key: row.capability_key,
        role: row.capability_role || "unknown",
        unit: row.unit || "",
        writable: normalizeBool(row.writable) === true,
        readable: normalizeBool(row.readable) !== false
    }));

const sensorMetrics = [];
const chartMetricCandidates = [];
const primaryValues = [];
const secondaryValues = [];
const seenKeys = new Set();

for (const capability of capabilities) {
    const currentValue = flatState[capability.key];
    seenKeys.add(capability.key);
    const row = {
        key: capability.key,
        label: humanizeKey(capability.key),
        value_text: formatValueText(capability.key, currentValue, capability.unit, capability.role),
        raw_value: currentValue
    };
    if (capability.role === "sensor") {
        primaryValues.push(row);
        sensorMetrics.push({ key: capability.key, label: row.label, unit: capability.unit || null });
        chartMetricCandidates.push({ key: capability.key, label: row.label, unit: capability.unit || null });
    } else {
        secondaryValues.push(row);
    }
}

for (const [key, value] of Object.entries(flatState)) {
    if (seenKeys.has(key)) continue;
    const row = { key, label: humanizeKey(key), value_text: formatValueText(key, value, null, "unknown"), raw_value: value };
    if (/temp|hum|lux|battery|power|energy|position/i.test(key) && !/cover_state/i.test(key)) {
        primaryValues.push(row);
        if (!/cover_position/i.test(key)) {
            sensorMetrics.push({ key, label: row.label, unit: null });
            chartMetricCandidates.push({ key, label: row.label, unit: null });
        }
    } else {
        secondaryValues.push(row);
    }
}

sortValues(primaryValues);
sortValues(secondaryValues);

const relayKeys = ["relay_1", "relay_2"].filter((key) => flatState[key] !== undefined || capabilities.some((item) => item.key === key));
const isRollladen = isRollladenDevice(flatState, meta, { device_class: deviceRow.device_class, hardware_type: deviceRow.hardware_type });
let controls = null;
if (isRollladen && relayKeys.length >= 2) {
    controls = buildCoverControls(flatState, normalizeBool(deviceRow.cover_calibrated) === true);
} else if (relayKeys.length) {
    controls = buildRelayControls(relayKeys, flatState, relayNameConfig);
}

const controlKind = controls ? controls.kind : null;
const visual = classifyDeviceVisual({
    device_role: deviceRow.device_role,
    device_class: deviceRow.device_class,
    hardware_type: deviceRow.hardware_type,
    power_source: deviceRow.power_source
}, { controlKind, relayCount: relayKeys.length, sensorCount: sensorMetrics.length });
const primaryRows = filterPrimaryValues(primaryValues, { controlKind }).slice(0, 6);

result.device = {
    device_id: deviceRow.device_id,
    device_role: deviceRow.device_role || "node",
    device_class: deviceRow.device_class || "unknown",
    display_name: displayName,
    display_name_input: uiDisplayName || normalizeString(deviceRow.display_name) || "",
    room_name: normalizeString(deviceRow.room_name) || normalizeString(state.room) || normalizeString(meta.room) || "",
    power_icon: deviceRow.power_source === "battery" ? "mdi-battery" : deviceRow.power_source === "mains" ? "mdi-power-plug" : "mdi-help-circle-outline",
    power_label: deviceRow.power_source === "battery" ? "BAT" : deviceRow.power_source === "mains" ? "AC" : "?",
    online_label: onlineBool === true ? "online" : onlineBool === false ? "offline" : "unbekannt",
    online_class: onlineBool === true ? "sh-chip-online" : onlineBool === false ? "sh-chip-offline" : "sh-chip-unknown",
    kind_label: visual.kind_label,
    type_icon: visual.type_icon,
    surface_class: visual.surface_class,
    delete_supported: deviceRow.device_role !== "master",
    controls,
    summary_label: controlKind === "relay"
        ? ""
        : controlKind === "cover"
            ? controls.state_text
            : "",
    relay_name_fields: controlKind === "relay"
        ? relayKeys.map((key) => ({
            key,
            label: defaultRelayLabel(key),
            value: relayNameConfig[key] || "",
            placeholder: defaultRelayLabel(key)
        }))
        : [],
    primary_values: primaryRows,
    info_rows: [
        { key: "device_class", label: "Klasse", value_text: deviceRow.device_class || "-" },
        { key: "origin_master_id", label: "Master", value_text: deviceRow.origin_master_id || "-" },
        { key: "hardware_type", label: "Hardware", value_text: deviceRow.hardware_type || "-" },
        { key: "firmware_version", label: "Firmware", value_text: deviceRow.firmware_version || "-" },
        { key: "protocol_version", label: "Protokoll", value_text: deviceRow.protocol_version || "-" },
        { key: "last_seen_at", label: "Zuletzt gesehen", value_text: isoLabel(deviceRow.last_seen_at) }
    ]
};

result.sensor_metrics = sensorMetrics.filter((item, index, list) => list.findIndex((current) => current.key === item.key) === index);
result.chart_metrics = finalizeChartMetricOptions(chartMetricCandidates);
result.chart_unavailable_reason = result.sensor_metrics.length > 0 && !result.chart_metrics.length
    ? "Dieses Gerät meldet Sensorwerte, aber keine im aktuellen Influx-Schema chartbaren Zeitreihen."
    : "";
result.technical_rows = [
    { key: "first_seen_at", label: "Erstkontakt", value_text: isoLabel(deviceRow.first_seen_at) },
    { key: "last_meta_at", label: "Letztes Meta", value_text: isoLabel(deviceRow.last_meta_at) },
    { key: "last_status_at", label: "Letzter Status", value_text: isoLabel(deviceRow.last_status_at) },
    { key: "last_state_at", label: "Letzter State", value_text: isoLabel(deviceRow.last_state_at) },
    { key: "manufacturer", label: "Hersteller", value_text: normalizeString(deviceRow.manufacturer) || "-" },
    { key: "model", label: "Modell", value_text: normalizeString(deviceRow.model) || "-" }
];
result.config_rows = rows
    .filter((row) => row.row_type === "config" && row.config_key && row.config_source === "cfg/report")
    .map((row) => ({
        key: row.config_key,
        value_text: row.config_type === "json" ? (row.config_value || "-") : (row.config_value || "-"),
        source: row.config_source || "-"
    }));

msg.payload = result;
return msg;
`;

const buildDeviceSaveStatementsFunc = String.raw`
${toSqlLines.join("\n")}
const normalizeString = (value) => typeof value === "string" ? value.trim() : "";
const slugify = (value) => normalizeString(value).toLowerCase().replace(/[^a-z0-9]+/g, "-").replace(/^-+|-+$/g, "");
const request = msg.payload || {};
const deviceId = typeof request.device_id === "string" ? request.device_id.trim() : "";
if (!deviceId) {
    return null;
}
const displayName = normalizeString(request.display_name);
const roomName = normalizeString(request.room_name);
const roomSlug = slugify(roomName);
const relayNames = request.relay_names && typeof request.relay_names === "object" ? request.relay_names : {};
const requestedAt = new Date().toISOString();
const messages = [];

if (displayName) {
    messages.push({
        topic: [
            "INSERT INTO device_config (device_id, config_key, config_value, config_type, source_channel, last_requested_at, updated_at)",
            "VALUES (" + [toSql(deviceId), toSql("ui_display_name"), toSql(displayName), toSql("text"), toSql("server/ui"), toSql(requestedAt), toSql(requestedAt)].join(", ") + ")",
            "ON CONFLICT(device_id, config_key) DO UPDATE SET",
            "config_value = excluded.config_value,",
            "config_type = excluded.config_type,",
            "source_channel = excluded.source_channel,",
            "last_requested_at = excluded.last_requested_at,",
            "updated_at = excluded.updated_at"
        ].join(" ")
    });
} else {
    messages.push({
        topic: "DELETE FROM device_config WHERE device_id = " + toSql(deviceId) + " AND config_key = 'ui_display_name';"
    });
}

[
    { key: "relay_1", configKey: "ui_relay_1_name" },
    { key: "relay_2", configKey: "ui_relay_2_name" }
].forEach((entry) => {
    if (!Object.prototype.hasOwnProperty.call(relayNames, entry.key)) {
        return;
    }
    const relayName = normalizeString(relayNames[entry.key]);
    if (relayName) {
        messages.push({
            topic: [
                "INSERT INTO device_config (device_id, config_key, config_value, config_type, source_channel, last_requested_at, updated_at)",
                "VALUES (" + [toSql(deviceId), toSql(entry.configKey), toSql(relayName), toSql("text"), toSql("server/ui"), toSql(requestedAt), toSql(requestedAt)].join(", ") + ")",
                "ON CONFLICT(device_id, config_key) DO UPDATE SET",
                "config_value = excluded.config_value,",
                "config_type = excluded.config_type,",
                "source_channel = excluded.source_channel,",
                "last_requested_at = excluded.last_requested_at,",
                "updated_at = excluded.updated_at"
            ].join(" ")
        });
        return;
    }
    messages.push({
        topic: "DELETE FROM device_config WHERE device_id = " + toSql(deviceId) + " AND config_key = " + toSql(entry.configKey) + ";"
    });
});

if (roomName && roomSlug) {
    messages.push({
        topic: [
            "INSERT INTO rooms (slug, name, sort_order, created_at, updated_at)",
            "VALUES (" + [toSql(roomSlug), toSql(roomName), "0", "CURRENT_TIMESTAMP", "CURRENT_TIMESTAMP"].join(", ") + ")",
            "ON CONFLICT(slug) DO UPDATE SET",
            "name = excluded.name,",
            "updated_at = CURRENT_TIMESTAMP"
        ].join(" ")
    });
    messages.push({
        topic: "UPDATE devices SET room_id = (SELECT id FROM rooms WHERE slug = " + toSql(roomSlug) + "), updated_at = CURRENT_TIMESTAMP WHERE device_id = " + toSql(deviceId) + ";",
        dashboardReload: { type: "detail", device_id: deviceId }
    });
} else {
    messages.push({
        topic: "UPDATE devices SET room_id = NULL, updated_at = CURRENT_TIMESTAMP WHERE device_id = " + toSql(deviceId) + ";",
        dashboardReload: { type: "detail", device_id: deviceId }
    });
}

return [messages];
`;

const buildDeviceDeleteStatementsFunc = String.raw`
${toSqlLines.join("\n")}
const request = msg.payload || {};
const deviceId = typeof request.device_id === "string" ? request.device_id.trim() : "";
if (!deviceId) {
    return null;
}

const messages = [
    { topic: "DELETE FROM audit_log WHERE device_id = " + toSql(deviceId) + ";" },
    { topic: "DELETE FROM device_capabilities WHERE device_id = " + toSql(deviceId) + ";" },
    { topic: "DELETE FROM device_config WHERE device_id = " + toSql(deviceId) + ";" },
    { topic: "DELETE FROM device_last_state WHERE device_id = " + toSql(deviceId) + ";" },
    {
        topic: "DELETE FROM devices WHERE device_id = " + toSql(deviceId) + ";",
        dashboardRemoved: true
    }
];

return [messages];
`;

const routePostDeviceDetailWriteFunc = String.raw`
if (msg.dashboardRemoved) {
    msg.payload = {
        kind: "detail_removed",
        message: "Gerät aus der SQLite-Registry entfernt. Neue MQTT-Nachrichten können es erneut anlegen."
    };
    return [msg, null];
}
if (!msg.dashboardReload || msg.dashboardReload.type !== "detail") {
    return [null, null];
}
msg.detailDeviceId = msg.dashboardReload.device_id;
return [null, msg];
`;

const routeWeatherActionFunc = String.raw`
const request = msg.payload || {};
if (request.action === "save") {
    return [null, msg];
}
return [msg, null];
`;

const buildWeatherQueryFunc = String.raw`
msg.topic = [
    "SELECT",
    "    provider,",
    "    location_label,",
    "    latitude,",
    "    longitude,",
    "    api_base_url,",
    "    poll_interval_minutes,",
    "    enabled,",
    "    units,",
    "    updated_at",
    "FROM weather_settings",
    "WHERE id = 1;"
].join(" ");
return msg;
`;

const buildWeatherPayloadFunc = String.raw`
const rows = Array.isArray(msg.payload) ? msg.payload : [];
const row = rows[0] || {};
msg.payload = {
    kind: "weather",
    provider: row.provider || "open-meteo",
    location_label: row.location_label || "",
    latitude: row.latitude,
    longitude: row.longitude,
    api_base_url: row.api_base_url || "",
    poll_interval_minutes: row.poll_interval_minutes || 15,
    enabled: row.enabled === 1 || row.enabled === true || row.enabled === "1",
    units: row.units || "metric",
    updated_at: row.updated_at || "-",
    note: "Aktuell pflegt diese Seite die Wetterkonfiguration. Ein produktiver Wetterdatenfeed ist serverseitig noch nicht vorhanden."
};
return msg;
`;

const buildWeatherSaveStatementsFunc = String.raw`
${toSqlLines.join("\n")}
const request = msg.payload || {};
const provider = typeof request.provider === "string" && request.provider.trim() ? request.provider.trim() : "open-meteo";
const locationLabel = typeof request.location_label === "string" ? request.location_label.trim() : "";
const latitude = request.latitude === "" || request.latitude === null || request.latitude === undefined ? null : Number(request.latitude);
const longitude = request.longitude === "" || request.longitude === null || request.longitude === undefined ? null : Number(request.longitude);
const pollMinutes = Number.isFinite(Number(request.poll_interval_minutes)) ? Math.max(1, Number(request.poll_interval_minutes)) : 15;
const units = typeof request.units === "string" && request.units.trim() ? request.units.trim() : "metric";
const enabled = request.enabled === true || request.enabled === "true" || request.enabled === 1 || request.enabled === "1";

return [[
    {
        topic: [
            "UPDATE weather_settings SET",
            "provider = " + toSql(provider) + ",",
            "location_label = " + toSql(locationLabel || null) + ",",
            "latitude = " + toSql(Number.isFinite(latitude) ? latitude : null) + ",",
            "longitude = " + toSql(Number.isFinite(longitude) ? longitude : null) + ",",
            "poll_interval_minutes = " + toSql(pollMinutes) + ",",
            "enabled = " + toSql(enabled ? 1 : 0) + ",",
            "units = " + toSql(units) + ",",
            "config_json = " + toSql(JSON.stringify({ provider, location_label: locationLabel || null, latitude: Number.isFinite(latitude) ? latitude : null, longitude: Number.isFinite(longitude) ? longitude : null, units })) + ",",
            "updated_at = CURRENT_TIMESTAMP",
            "WHERE id = 1;"
        ].join(" "),
        dashboardReload: { type: "weather" }
    }
]];
`;

const routePostWeatherWriteFunc = String.raw`
if (!msg.dashboardReload || msg.dashboardReload.type !== "weather") {
    return null;
}
return msg;
`;

const routeHeaderActionFunc = String.raw`
const request = msg.payload || {};
if (request.action === "load") {
    return msg;
}
return null;
`;

const buildHeaderQueryFunc = String.raw`
msg.topic = [
    "SELECT",
    "    provider,",
    "    location_label,",
    "    latitude,",
    "    longitude,",
    "    poll_interval_minutes,",
    "    enabled,",
    "    units,",
    "    updated_at",
    "FROM weather_settings",
    "WHERE id = 1;"
].join(" ");
return msg;
`;

const buildHeaderPayloadFunc = String.raw`
${dashboardCommonHelperLines.join("\n")}
const rows = Array.isArray(msg.payload) ? msg.payload : [];
const row = rows[0] || {};
const latitude = normalizeNumber(row.latitude);
const longitude = normalizeNumber(row.longitude);
const locationLabel = normalizeString(row.location_label);
const coordsLabel = latitude !== null && longitude !== null
    ? String(latitude) + ", " + String(longitude)
    : "";
const updatedAtText = normalizeString(row.updated_at);
const updatedAtIso = updatedAtText && !updatedAtText.includes("T")
    ? updatedAtText.replace(" ", "T") + "Z"
    : updatedAtText;
const enabled = normalizeBool(row.enabled) === true;
const configured = !!locationLabel || !!coordsLabel;

msg.payload = {
    kind: "header",
    notifications: {
        count: 0,
        label: "V1 hat bewusst noch keinen Benachrichtigungskanal. Der Header-Platzhalter ist nur die vorbereitete UI-Spur."
    },
    weather: {
        enabled,
        configured,
        provider_label: normalizeString(row.provider) || "open-meteo",
        location_label: locationLabel,
        coords_label: coordsLabel,
        poll_label: String(normalizeNumber(row.poll_interval_minutes) || 15) + " min",
        units_label: normalizeString(row.units) || "metric",
        updated_at: updatedAtIso,
        age_fallback: updatedAtIso ? formatRelativeSeenLabel(updatedAtIso) : "Stand -",
        status_note: enabled
            ? (configured ? "Kein Live-Feed" : "Kein Ort gesetzt")
            : "Noch nicht aktiviert",
        icon_mode: enabled
            ? (configured ? "configured" : "pending")
            : "inactive",
        tooltip: enabled
            ? "Wetter-Setup ist aktiv, aber der produktive Live-Feed ist im aktuellen Serverpfad noch nicht vorhanden."
            : "Wetter ist aktuell nicht aktiviert."
    }
};
return msg;
`;

const routeAutomationActionFunc = String.raw`
const request = msg.payload || {};
if (request.action === "save" || request.action === "toggle" || request.action === "delete") {
    return [null, msg];
}
return [msg, null];
`;

const buildAutomationQueryFunc = String.raw`
msg.topic = [
    "SELECT",
    "    'automation' AS row_type,",
    "    CAST(a.id AS TEXT) AS automation_id,",
    "    a.name,",
    "    a.is_enabled,",
    "    a.trigger_type,",
    "    a.trigger_definition_json,",
    "    a.action_definition_json,",
    "    a.notes,",
    "    a.last_executed_at,",
    "    NULL AS device_id,",
    "    NULL AS display_name,",
    "    NULL AS capability_key,",
    "    NULL AS unit,",
    "    NULL AS state_json",
    "FROM automations AS a",
    "UNION ALL",
    "SELECT",
    "    'sensor' AS row_type,",
    "    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,",
    "    d.device_id,",
    "    COALESCE(cfg_name.config_value, d.display_name, d.device_id) AS display_name,",
    "    dc.capability_key,",
    "    dc.unit,",
    "    NULL",
    "FROM devices AS d",
    "JOIN device_capabilities AS dc ON dc.device_id = d.device_id",
    "LEFT JOIN device_config AS cfg_name ON cfg_name.device_id = d.device_id AND cfg_name.config_key = 'ui_display_name'",
    "WHERE dc.capability_role = 'sensor'",
    "UNION ALL",
    "SELECT",
    "    'actuator' AS row_type,",
    "    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,",
    "    d.device_id,",
    "    COALESCE(cfg_name.config_value, d.display_name, d.device_id) AS display_name,",
    "    dc.capability_key,",
    "    dc.unit,",
    "    ls.state_json",
    "FROM devices AS d",
    "JOIN device_capabilities AS dc ON dc.device_id = d.device_id",
    "LEFT JOIN device_last_state AS ls ON ls.device_id = d.device_id",
    "LEFT JOIN device_config AS cfg_name ON cfg_name.device_id = d.device_id AND cfg_name.config_key = 'ui_display_name'",
    "WHERE dc.capability_role = 'actuator'",
    "ORDER BY row_type, display_name, capability_key;"
].join(" ");
return msg;
`;

const buildAutomationPayloadFunc = String.raw`
${dashboardCommonHelperLines.join("\n")}
const rows = Array.isArray(msg.payload) ? msg.payload : [];
const sensorMap = new Map();
const actuatorMap = new Map();
const automations = [];

for (const row of rows) {
    if (row.row_type === "sensor" && row.device_id && row.capability_key) {
        if (!sensorMap.has(row.device_id)) {
            sensorMap.set(row.device_id, { device_id: row.device_id, display_name: row.display_name || row.device_id, metrics: [] });
        }
        sensorMap.get(row.device_id).metrics.push({ key: row.capability_key, label: humanizeKey(row.capability_key), unit: row.unit || null });
    }
    if (row.row_type === "actuator" && row.device_id && row.capability_key) {
        if (!actuatorMap.has(row.device_id)) {
            actuatorMap.set(row.device_id, {
                device_id: row.device_id,
                display_name: row.display_name || row.device_id,
                actions: [],
                relay_keys: new Set(),
                state_json: parseJson(row.state_json) || {}
            });
        }
        actuatorMap.get(row.device_id).relay_keys.add(row.capability_key);
    }
}

for (const entry of actuatorMap.values()) {
    for (const relayKey of Array.from(entry.relay_keys).sort()) {
        entry.actions.push({ code: relayKey + ":on", label: humanizeKey(relayKey) + " EIN" });
        entry.actions.push({ code: relayKey + ":off", label: humanizeKey(relayKey) + " AUS" });
    }
    const flatState = flattenState(entry.state_json || {});
    if (entry.relay_keys.has("relay_1") && entry.relay_keys.has("relay_2") && normalizeBool(flatState.cover_mode) === true) {
        entry.actions.push({ code: "cover:up", label: "Rollladen auf" });
        entry.actions.push({ code: "cover:stop", label: "Rollladen Stopp" });
        entry.actions.push({ code: "cover:down", label: "Rollladen ab" });
    }
    entry.actions.sort((left, right) => left.label.localeCompare(right.label));
}

for (const row of rows.filter((item) => item.row_type === "automation")) {
    const trigger = parseJson(row.trigger_definition_json) || {};
    const action = parseJson(row.action_definition_json) || {};
    const triggerLabel = row.trigger_type === "time"
        ? "Zeit " + (trigger.at || "--:--")
        : "Sensor " + [trigger.device_id || "-", trigger.metric || "-", trigger.operator || "-", trigger.threshold].join(" ");
    const coverDirectionLabel = action.direction === "stop" ? "Stopp" : action.direction || "-";
    const actionLabel = action.type === "relay"
        ? [action.device_id || "-", action.relay_key || "-", action.state === true ? "EIN" : "AUS"].join(" ")
        : action.type === "cover"
            ? [action.device_id || "-", "Rollladen", coverDirectionLabel].join(" ")
            : "-";
    automations.push({
        id: Number(row.automation_id),
        name: row.name || "ohne Name",
        trigger_label: triggerLabel,
        action_label: actionLabel,
        is_enabled: row.is_enabled === 1 || row.is_enabled === true || row.is_enabled === "1",
        last_executed_at: row.last_executed_at || "",
        form_values: {
            id: Number(row.automation_id),
            name: row.name || "",
            trigger_type: row.trigger_type || "time",
            time_at: trigger.at || "07:00",
            sensor_device_id: trigger.device_id || "",
            sensor_metric: trigger.metric || "",
            sensor_operator: trigger.operator || ">=",
            sensor_threshold: trigger.threshold ?? "",
            action_device_id: action.device_id || "",
            action_code: action.type === "relay"
                ? action.relay_key + ":" + (action.state === true ? "on" : "off")
                : action.type === "cover"
                    ? "cover:" + (action.direction || "stop")
                    : "",
            notes: row.notes || "",
            is_enabled: row.is_enabled === 1 || row.is_enabled === true || row.is_enabled === "1"
        }
    });
}

msg.payload = {
    kind: "automations",
    sensor_devices: Array.from(sensorMap.values()).sort((left, right) => left.display_name.localeCompare(right.display_name)),
    actuator_devices: Array.from(actuatorMap.values()).sort((left, right) => left.display_name.localeCompare(right.display_name)),
    automations
};
return msg;
`;

const buildAutomationWriteStatementsFunc = String.raw`
${toSqlLines.join("\n")}
const request = msg.payload || {};
const action = typeof request.action === "string" ? request.action : "";
if (!action) {
    return null;
}
if (action === "toggle") {
    return [[{
        topic: "UPDATE automations SET is_enabled = " + toSql(request.is_enabled ? 1 : 0) + ", updated_at = CURRENT_TIMESTAMP WHERE id = " + toSql(Number(request.id)) + ";",
        dashboardReload: { type: "automations" }
    }]];
}
if (action === "delete") {
    return [[{
        topic: "DELETE FROM automations WHERE id = " + toSql(Number(request.id)) + ";",
        dashboardReload: { type: "automations" }
    }]];
}
const name = typeof request.name === "string" ? request.name.trim() : "";
if (!name) {
    return null;
}
const triggerDefinition = request.trigger_type === "sensor"
    ? {
        device_id: request.sensor_device_id || "",
        metric: request.sensor_metric || "",
        operator: request.sensor_operator || ">=",
        threshold: Number(request.sensor_threshold)
    }
    : { at: request.time_at || "07:00" };
let actionDefinition = {};
if (typeof request.action_code === "string" && request.action_code.startsWith("cover:")) {
    actionDefinition = {
        type: "cover",
        device_id: request.action_device_id || "",
        direction: request.action_code.split(":")[1] || "stop"
    };
} else {
    const actionParts = typeof request.action_code === "string" ? request.action_code.split(":") : [];
    actionDefinition = {
        type: "relay",
        device_id: request.action_device_id || "",
        relay_key: actionParts[0] || "relay_1",
        state: actionParts[1] === "on"
    };
}
const notes = typeof request.notes === "string" ? request.notes.trim() : "";
if (request.id) {
    return [[{
        topic: [
            "UPDATE automations SET",
            "name = " + toSql(name) + ",",
            "is_enabled = " + toSql(request.is_enabled ? 1 : 0) + ",",
            "trigger_type = " + toSql(request.trigger_type === "sensor" ? "sensor" : "time") + ",",
            "trigger_definition_json = " + toSql(JSON.stringify(triggerDefinition)) + ",",
            "action_definition_json = " + toSql(JSON.stringify(actionDefinition)) + ",",
            "notes = " + toSql(notes || null) + ",",
            "updated_at = CURRENT_TIMESTAMP",
            "WHERE id = " + toSql(Number(request.id)) + ";"
        ].join(" "),
        dashboardReload: { type: "automations" }
    }]];
}
return [[{
    topic: [
        "INSERT INTO automations (name, is_enabled, trigger_type, trigger_definition_json, action_definition_json, notes, created_at, updated_at)",
        "VALUES (" + [
            toSql(name),
            toSql(request.is_enabled ? 1 : 0),
            toSql(request.trigger_type === "sensor" ? "sensor" : "time"),
            toSql(JSON.stringify(triggerDefinition)),
            toSql(JSON.stringify(actionDefinition)),
            toSql(notes || null),
            "CURRENT_TIMESTAMP",
            "CURRENT_TIMESTAMP"
        ].join(", ") + ");"
    ].join(" "),
    dashboardReload: { type: "automations" }
}]];
`;

const routePostAutomationWriteFunc = String.raw`
if (!msg.dashboardReload || msg.dashboardReload.type !== "automations") {
    return null;
}
return msg;
`;

const routeLogsActionFunc = String.raw`
return msg;
`;

const buildLogsQueryFunc = String.raw`
msg.topic = [
    "SELECT id, occurred_at, direction, scope, channel, topic,",
    "       COALESCE(device_id, master_id, 'server') AS entity_label,",
    "       payload_json",
    "FROM audit_log",
    "ORDER BY occurred_at DESC",
    "LIMIT 120;"
].join(" ");
return msg;
`;

const buildLogsPayloadFunc = String.raw`
const rows = Array.isArray(msg.payload) ? msg.payload : [];
msg.payload = {
    rows: rows.map((row) => ({
        id: row.id,
        occurred_at: row.occurred_at || "-",
        direction: row.direction || "-",
        scope: row.scope || "-",
        channel: row.channel || "-",
        topic: row.topic || "-",
        entity_label: row.entity_label || "-",
        payload_preview: row.payload_json || "-"
    }))
};
return msg;
`;

const routeConfigActionFunc = String.raw`
return msg;
`;

const buildConfigPageQueryFunc = String.raw`
msg.topic = [
    "SELECT 'setting' AS row_type, setting_key AS item_key, setting_value AS item_value, value_type, updated_at, NULL AS provider, NULL AS location_label, NULL AS poll_interval_minutes, NULL AS room_slug, NULL AS room_name",
    "FROM server_settings",
    "UNION ALL",
    "SELECT 'weather' AS row_type, NULL, NULL, NULL, updated_at, provider, location_label, CAST(poll_interval_minutes AS TEXT), NULL, NULL",
    "FROM weather_settings WHERE id = 1",
    "UNION ALL",
    "SELECT 'device' AS row_type, d.device_id AS item_key, COALESCE(cfg_name.config_value, d.display_name, d.device_id) AS item_value, d.hardware_type AS value_type, d.updated_at, NULL, NULL, NULL, NULL, NULL",
    "FROM devices AS d",
    "LEFT JOIN device_config AS cfg_name ON cfg_name.device_id = d.device_id AND cfg_name.config_key = 'ui_display_name'",
    "UNION ALL",
    "SELECT 'room' AS row_type, NULL, NULL, NULL, updated_at, NULL, NULL, NULL, slug, name",
    "FROM rooms",
    "ORDER BY row_type, item_value, item_key, room_name;"
].join(" ");
return msg;
`;

const buildConfigPagePayloadFunc = String.raw`
const rows = Array.isArray(msg.payload) ? msg.payload : [];
const settings = [];
const rooms = [];
const devices = [];
let weather = {};
let serverStatusPretty = "";

for (const row of rows) {
    if (row.row_type === "setting") {
        settings.push({
            key: row.item_key,
            value_text: row.item_value === null || row.item_value === undefined || row.item_value === "" ? "-" : String(row.item_value),
            value_type: row.value_type || "-"
        });
        if (row.item_key === "server_status_json" && row.item_value) {
            try {
                serverStatusPretty = JSON.stringify(JSON.parse(row.item_value), null, 2);
            } catch (error) {
                serverStatusPretty = String(row.item_value);
            }
        }
    } else if (row.row_type === "weather") {
        weather = {
            provider: row.provider || "-",
            location_label: row.location_label || "",
            poll_label: row.poll_interval_minutes ? String(row.poll_interval_minutes) + " min" : "-"
        };
    } else if (row.row_type === "device" && row.item_key) {
        devices.push({
            device_id: row.item_key,
            display_name: row.item_value || row.item_key,
            hardware_type: row.value_type || ""
        });
    } else if (row.row_type === "room" && row.room_slug) {
        rooms.push({ slug: row.room_slug, name: row.room_name || row.room_slug });
    }
}

msg.payload = {
    kind: "config",
    settings,
    devices,
    rooms,
    weather,
    server_status_pretty: serverStatusPretty
};
return msg;
`;

const buildChartRequestFunc = String.raw`
${dashboardCommonHelperLines.join("\n")}
const request = msg.payload || {};
const deviceId = typeof request.device_id === "string" ? request.device_id.trim() : "";
const deviceLabel = normalizeString(request.device_label) || deviceId;
const requestedMetrics = Array.isArray(request.metrics) ? request.metrics.filter((item) => typeof item === "string" && item.trim()) : [];
const metrics = Array.from(new Set(requestedMetrics
    .map((item) => normalizeMetricKey(item))
    .filter((item) => item && isChartableMetric(item))));
msg.chartTarget = request.chart_target === "charts" ? "charts" : "detail";
const rangeMap = { "1h": "-1h", "6h": "-6h", "24h": "-24h", "7d": "-7d", "30d": "-30d" };
const autoStepMap = { "1h": "raw", "6h": "raw", "24h": "raw", "7d": "15m", "30d": "1h" };
const allowedSteps = new Set(["raw", "10s", "30s", "1m", "5m", "15m", "1h"]);
const rangeKey = Object.prototype.hasOwnProperty.call(rangeMap, request.range) ? request.range : "24h";
const requestedStep = typeof request.step === "string" ? request.step.trim() : "auto";
const stepKey = requestedStep && requestedStep !== "auto" && allowedSteps.has(requestedStep) ? requestedStep : autoStepMap[rangeKey];
const metricSummary = formatChartMetricSummary(metrics.length ? metrics : requestedMetrics);
const chartMeta = {
    title: deviceLabel || "",
    range_label: formatChartRangeLabel(rangeKey),
    step_label: formatChartStepLabel(stepKey),
    device_id: deviceId,
    metric_summary: metricSummary
};
if (!deviceId || !metrics.length) {
    msg.payload = {
        kind: "chart",
        chart: buildChartState(
            "invalid_selection",
            !deviceId
                ? "Bitte zuerst ein Gerät wählen."
                : requestedMetrics.length
                    ? "Die ausgewählte Metrik ist im aktuellen Influx-Schema nicht chartbar."
                    : "Bitte mindestens eine Metrik wählen.",
            chartMeta
        )
    };
    return [null, msg];
}
const escapedDeviceId = deviceId.replace(/"/g, '\\"');
const metricFilter = metrics.map((metric) => 'r.metric == "' + String(metric).replace(/"/g, '\\"') + '"').join(" or ");
const fluxQueryLines = [
    'from(bucket: "' + String(env.get("INFLUX_BUCKET") || "smarthome").replace(/"/g, '\\"') + '")',
    '  |> range(start: ' + rangeMap[rangeKey] + ')',
    '  |> filter(fn: (r) => r._measurement == "smarthome_sensor" and r.node_id == "' + escapedDeviceId + '" and (' + metricFilter + '))'
];
if (stepKey !== "raw") {
    fluxQueryLines.push('  |> aggregateWindow(every: ' + stepKey + ', fn: mean, createEmpty: false)');
}
fluxQueryLines.push(
    '  |> keep(columns: ["_time", "_value", "metric", "unit"])',
    '  |> sort(columns: ["_time"])'
);
msg.chartTitle = deviceLabel;
msg.chartDeviceId = deviceId;
msg.chartRangeLabel = chartMeta.range_label;
msg.chartStepLabel = chartMeta.step_label;
msg.chartMetricSummary = metricSummary;
msg.method = "POST";
msg.url = (env.get("INFLUX_URL") || "http://influxdb:8086") + "/api/v2/query?org=" + encodeURIComponent(env.get("INFLUX_ORG") || "");
msg.headers = {
    Authorization: "Token " + (env.get("INFLUX_TOKEN") || ""),
    "Content-Type": "application/json",
    Accept: "application/csv"
};
msg.payload = JSON.stringify({
    query: fluxQueryLines.join("\n"),
    dialect: {
        header: true,
        delimiter: ",",
        annotations: []
    }
});
return [msg, null];
`;

const parseChartResponseFunc = String.raw`
${dashboardCommonHelperLines.join("\n")}
const extractChartErrorMessage = () => {
    const statusCode = normalizeNumber(msg.statusCode);
    const rawPayload = typeof msg.payload === "string" ? msg.payload.trim() : "";
    if (statusCode !== null && statusCode >= 400) {
        const parsedError = rawPayload ? parseJson(rawPayload) : null;
        const detail = normalizeString(parsedError && (parsedError.message || parsedError.error))
            || rawPayload
            || (normalizeString(msg.statusMessage))
            || "Unbekannter Influx-Fehler.";
        return "Influx-Query fehlgeschlagen (HTTP " + String(Math.round(statusCode)) + "): " + detail;
    }
    return null;
};
const chartErrorMessage = extractChartErrorMessage();
if (chartErrorMessage) {
    msg.payload = {
        kind: "chart",
        chart: buildChartState("query_error", chartErrorMessage, {
            title: msg.chartTitle || "Chart",
            range_label: msg.chartRangeLabel || "-",
            step_label: msg.chartStepLabel || "-",
            device_id: msg.chartDeviceId || "",
            metric_summary: msg.chartMetricSummary || ""
        })
    };
    return msg;
}
const csv = (
    typeof msg.payload === "string"
        ? msg.payload
        : Buffer.isBuffer(msg.payload)
            ? msg.payload.toString("utf8")
            : ""
).trim();
if (!csv) {
    msg.payload = {
        kind: "chart",
        chart: buildChartState("no_data", "Keine Influx-Daten für die aktuelle Auswahl.", {
            title: msg.chartTitle || "Chart",
            range_label: msg.chartRangeLabel || "-",
            step_label: msg.chartStepLabel || "-",
            device_id: msg.chartDeviceId || "",
            metric_summary: msg.chartMetricSummary || ""
        })
    };
    return msg;
}
const lines = csv.split(/\r?\n/).filter((line) => line.trim());
if (!lines.length) {
    msg.payload = {
        kind: "chart",
        chart: buildChartState("no_data", "Keine Influx-Daten für die aktuelle Auswahl.", {
            title: msg.chartTitle || "Chart",
            range_label: msg.chartRangeLabel || "-",
            step_label: msg.chartStepLabel || "-",
            device_id: msg.chartDeviceId || "",
            metric_summary: msg.chartMetricSummary || ""
        })
    };
    return msg;
}
const splitCsvLine = (line) => {
    const values = [];
    let current = "";
    let inQuotes = false;
    for (let index = 0; index < line.length; index += 1) {
        const character = line[index];
        if (character === '"') {
            if (inQuotes && line[index + 1] === '"') {
                current += '"';
                index += 1;
            } else {
                inQuotes = !inQuotes;
            }
            continue;
        }
        if (character === "," && !inQuotes) {
            values.push(current);
            current = "";
            continue;
        }
        current += character;
    }
    values.push(current);
    return values;
};
const relevantLines = lines.filter((line) => !line.startsWith("#"));
const headerLine = relevantLines.find((line) => line.includes("_time") && line.includes("_value") && line.includes("metric"));
if (!headerLine) {
    msg.payload = {
        kind: "chart",
        chart: buildChartState("render_error", "Influx-Antwort ohne lesbaren CSV-Header.", {
            title: msg.chartTitle || "Chart",
            range_label: msg.chartRangeLabel || "-",
            step_label: msg.chartStepLabel || "-",
            device_id: msg.chartDeviceId || "",
            metric_summary: msg.chartMetricSummary || ""
        })
    };
    return msg;
}
const headers = splitCsvLine(headerLine);
const headerIndex = {};
headers.forEach((header, index) => { headerIndex[header] = index; });
const chartRows = relevantLines
    .filter((line) => line !== headerLine)
    .filter((line) => !(line.includes("_time") && line.includes("_value") && line.includes("metric")))
    .map((line) => {
    const values = splitCsvLine(line);
    return {
        ts: values[headerIndex._time] || "",
        value_num: values[headerIndex._value] || "",
        metric: values[headerIndex.metric] || "",
        unit: values[headerIndex.unit] || ""
    };
});
msg.payload = {
    kind: "chart",
    chart: buildChartSeries(
        chartRows,
        msg.chartTitle || "Chart",
        msg.chartRangeLabel || "-",
        msg.chartStepLabel || "-",
        msg.chartDeviceId || "",
        msg.chartMetricSummary || ""
    )
};
return msg;
`;

const routeChartPayloadFunc = String.raw`
if (msg.chartTarget === "charts") {
    return [null, msg];
}
return [msg, null];
`;

const buildSensorAutomationQueryFunc = String.raw`
msg.stateEvent = msg.smarthomeEvent || msg.payload || {};
msg.topic = "SELECT id, trigger_definition_json, action_definition_json FROM automations WHERE is_enabled = 1 AND trigger_type = 'sensor';";
return msg;
`;

const evaluateSensorAutomationsFunc = String.raw`
${dashboardCommonHelperLines.join("\n")}
const rows = Array.isArray(msg.payload) ? msg.payload : [];
const stateEvent = msg.stateEvent || {};
const flatState = flattenState(stateEvent.payload || {});
const flags = flow.get("sensor_automation_flags") || {};
const commandMessages = [];
const updateMessages = [];
const compare = (left, operator, right) => {
    if (operator === ">") return left > right;
    if (operator === "<") return left < right;
    if (operator === "<=") return left <= right;
    if (operator === "==") return left === right;
    return left >= right;
};

for (const row of rows) {
    const trigger = parseJson(row.trigger_definition_json) || {};
    const action = parseJson(row.action_definition_json) || {};
    if (trigger.device_id && trigger.device_id !== stateEvent.entityId) continue;
    const currentValue = normalizeNumber(flatState[trigger.metric]);
    const threshold = normalizeNumber(trigger.threshold);
    if (currentValue === null || threshold === null) continue;
    const active = compare(currentValue, trigger.operator || ">=", threshold);
    const stateKey = "automation_" + String(row.id);
    const wasActive = flags[stateKey] === true;
    flags[stateKey] = active;
    if (!active || wasActive) continue;
    if (action.type === "relay") {
        commandMessages.push({ payload: { action: "relay_set", view: "automations", device_id: action.device_id, relay_key: action.relay_key, state: action.state === true } });
    } else if (action.type === "cover") {
        commandMessages.push({ payload: { action: "cover_move", view: "automations", device_id: action.device_id, direction: action.direction || "stop" } });
    }
    updateMessages.push({ payload: { automation_id: row.id, executed_at: new Date().toISOString() } });
}

flow.set("sensor_automation_flags", flags);
return [commandMessages, updateMessages];
`;

const buildTimeAutomationQueryFunc = String.raw`
msg.topic = "SELECT id, trigger_definition_json, action_definition_json FROM automations WHERE is_enabled = 1 AND trigger_type = 'time';";
return msg;
`;

const evaluateTimeAutomationsFunc = String.raw`
${dashboardCommonHelperLines.join("\n")}
const rows = Array.isArray(msg.payload) ? msg.payload : [];
const now = new Date();
const currentMark = [now.getFullYear(), now.getMonth() + 1, now.getDate(), now.getHours(), now.getMinutes()].join("-");
const marks = flow.get("time_automation_marks") || {};
const commandMessages = [];
const updateMessages = [];

for (const row of rows) {
    const trigger = parseJson(row.trigger_definition_json) || {};
    const action = parseJson(row.action_definition_json) || {};
    if (!trigger.at || !/^\\d{2}:\\d{2}$/.test(trigger.at)) continue;
    const [hourText, minuteText] = trigger.at.split(":");
    if (Number(hourText) !== now.getHours() || Number(minuteText) !== now.getMinutes()) continue;
    const stateKey = "automation_" + String(row.id);
    if (marks[stateKey] === currentMark) continue;
    marks[stateKey] = currentMark;
    if (action.type === "relay") {
        commandMessages.push({ payload: { action: "relay_set", view: "automations", device_id: action.device_id, relay_key: action.relay_key, state: action.state === true } });
    } else if (action.type === "cover") {
        commandMessages.push({ payload: { action: "cover_move", view: "automations", device_id: action.device_id, direction: action.direction || "stop" } });
    }
    updateMessages.push({ payload: { automation_id: row.id, executed_at: new Date().toISOString() } });
}

flow.set("time_automation_marks", marks);
return [commandMessages, updateMessages];
`;

const buildAutomationExecutionUpdateFunc = String.raw`
${toSqlLines.join("\n")}
const request = msg.payload || {};
if (!request.automation_id) {
    return null;
}
msg.topic = "UPDATE automations SET last_executed_at = " + toSql(request.executed_at || new Date().toISOString()) + ", updated_at = CURRENT_TIMESTAMP WHERE id = " + toSql(Number(request.automation_id)) + ";";
return msg;
`;

addNode({
    id: ids.tabIngest,
    type: "tab",
    label: "10 MQTT Ingest",
    disabled: false,
    info: "Normalize MQTT topics and route server/master/node payloads."
});

addNode({
    id: ids.tabRegistry,
    type: "tab",
    label: "20 Registry",
    disabled: false,
    info: "Automatic device creation and capability upsert in SQLite."
});

addNode({
    id: ids.tabState,
    type: "tab",
    label: "30 State Store",
    disabled: false,
    info: "Separated status/state/event/ack and server status persistence."
});

addNode({
    id: ids.tabConfig,
    type: "tab",
    label: "35 Config Store",
    disabled: false,
    info: "Persist cfg/report payloads into device_config."
});

addNode({
    id: ids.tabTimeSeries,
    type: "tab",
    label: "40 Time Series",
    disabled: false,
    info: "Write only numeric sensor series from MQTT state into InfluxDB."
});

addNode({
    id: ids.tabAutomation,
    type: "tab",
    label: "45 Automations",
    disabled: false,
    info: "Simple time and sensor driven automation execution in Node-RED."
});

addNode({
    id: ids.tabCommand,
    type: "tab",
    label: "50 Command Egress",
    disabled: false,
    info: "Dashboard-triggered generic cmd/set publish plus the versioned proof injects."
});

addNode({
    id: ids.tabDashboardUi,
    type: "tab",
    label: "55 Dashboard V1",
    disabled: false,
    info: "Responsive FlowFuse dashboard pages for the real server stack."
});

addNode({
    id: ids.tabDashboardSim,
    type: "tab",
    label: "60 Dashboard [SIM]",
    disabled: false,
    info: "Read-only simulation dashboard query path for sim_* validation."
});

addNode({
    id: ids.tabLogging,
    type: "tab",
    label: "80 Logging",
    disabled: false,
    info: "Audit logging for all ingested MQTT messages."
});

addNode({
    id: ids.mqttBroker,
    type: "mqtt-broker",
    name: "SmartHome MQTT",
    broker: "${MQTT_HOST}",
    port: "${MQTT_PORT}",
    clientid: "smarthome-server-v1",
    autoConnect: true,
    usetls: false,
    protocolVersion: "5",
    keepalive: "60",
    cleansession: true,
    birthTopic: "",
    birthQos: "0",
    birthRetain: "false",
    birthPayload: "",
    birthMsg: {},
    closeTopic: "",
    closeQos: "0",
    closeRetain: "false",
    closePayload: "",
    closeMsg: {},
    willTopic: "",
    willQos: "0",
    willRetain: "false",
    willPayload: "",
    willMsg: {},
    userProps: "",
    sessionExpiry: ""
});

addNode({
    id: ids.sqliteDb,
    type: "sqlitedb",
    db: "${SQLITE_PATH}",
    mode: "RWC",
    name: "sqlite_db"
});

addNode({
    id: ids.uiBase,
    type: "ui-base",
    name: "SmartHome Dashboard",
    path: "/dashboard",
    appIcon: "",
    includeClientData: true,
    acceptsClientConfig: ["ui-notification", "ui-control"],
    showPathInSidebar: false,
    headerContent: "page",
    navigationStyle: "default",
    titleBarStyle: "default",
    showReconnectNotification: true,
    notificationDisplayTime: 1,
    showDisconnectNotification: true,
    allowInstall: false
});

addNode({
    id: ids.uiThemeDefault,
    type: "ui-theme",
    ui: ids.uiBase,
    name: "Standard",
    colors: {
        surface: "#111821",
        primary: "#36c6aa",
        bgPage: "#091017",
        groupBg: "#111821",
        groupOutline: "#2a3846"
    },
    sizes: {
        density: "default",
        pagePadding: "12px",
        groupGap: "12px",
        groupBorderRadius: "4px",
        widgetGap: "12px"
    }
});

[
    { id: ids.uiPageOverview, name: "Übersicht", path: "/", icon: "view-dashboard", order: 1, visible: true },
    { id: ids.uiPageRooms, name: "Räume", path: "/raeume", icon: "floor-plan", order: 2, visible: true },
    { id: ids.uiPageDevices, name: "Geräte", path: "/geraete", icon: "devices", order: 3, visible: true },
    { id: ids.uiPageDeviceDetail, name: "Gerätedetail", path: "/geraet", icon: "card-text-outline", order: 4, visible: false },
    { id: ids.uiPageSensors, name: "Sensoren", path: "/sensoren", icon: "thermometer", order: 5, visible: true },
    { id: ids.uiPageActors, name: "Aktoren", path: "/aktoren", icon: "toggle-switch", order: 6, visible: true },
    { id: ids.uiPageCovers, name: "Rollläden", path: "/rollaeden", icon: "window-shutter", order: 7, visible: true },
    { id: ids.uiPageWeather, name: "Wetter", path: "/wetter", icon: "weather-partly-cloudy", order: 8, visible: true },
    { id: ids.uiPageCharts, name: "Diagramme", path: "/diagramme", icon: "chart-line", order: 9, visible: true },
    { id: ids.uiPageAutomations, name: "Automationen", path: "/automationen", icon: "robot-outline", order: 10, visible: true },
    { id: ids.uiPageConfig, name: "Konfiguration", path: "/konfiguration", icon: "cog-outline", order: 11, visible: true },
    { id: ids.uiPageLogs, name: "Logs", path: "/logs", icon: "text-box-search-outline", order: 12, visible: true },
    { id: ids.uiPageSimulation, name: "[SIM] Validierung", path: "/simulation", icon: "flask-outline", order: 90, visible: false }
].forEach(({ id, name, path, icon, order, visible }) => {
    addNode({
        id,
        type: "ui-page",
        name,
        ui: ids.uiBase,
        path,
        icon,
        layout: "grid",
        theme: ids.uiThemeDefault,
        breakpoints: [
            { name: "Default", px: 0, cols: 3 },
            { name: "Tablet", px: 576, cols: 6 },
            { name: "Small Desktop", px: 768, cols: 9 },
            { name: "Desktop", px: 1024, cols: 12 }
        ],
        order,
        className: "",
        visible,
        disabled: false
    });
});

[
    { id: ids.uiGroupOverview, page: ids.uiPageOverview, name: "Übersicht", height: 18, order: 1 },
    { id: ids.uiGroupRooms, page: ids.uiPageRooms, name: "Räume", height: 14, order: 1 },
    { id: ids.uiGroupDevices, page: ids.uiPageDevices, name: "Geräte", height: 18, order: 1 },
    { id: ids.uiGroupDeviceDetail, page: ids.uiPageDeviceDetail, name: "Detail", height: 24, order: 1 },
    { id: ids.uiGroupSensors, page: ids.uiPageSensors, name: "Sensoren", height: 18, order: 1 },
    { id: ids.uiGroupActors, page: ids.uiPageActors, name: "Aktoren", height: 18, order: 1 },
    { id: ids.uiGroupCovers, page: ids.uiPageCovers, name: "Rollläden", height: 18, order: 1 },
    { id: ids.uiGroupWeather, page: ids.uiPageWeather, name: "Wetter", height: 12, order: 1 },
    { id: ids.uiGroupCharts, page: ids.uiPageCharts, name: "Diagramme", height: 16, order: 1 },
    { id: ids.uiGroupAutomations, page: ids.uiPageAutomations, name: "Automationen", height: 18, order: 1 },
    { id: ids.uiGroupConfig, page: ids.uiPageConfig, name: "Konfiguration", height: 18, order: 1 },
    { id: ids.uiGroupLogs, page: ids.uiPageLogs, name: "Logs", height: 18, order: 1 },
    { id: ids.uiGroupSimControls, page: ids.uiPageSimulation, name: "[SIM] Aktionen", height: 1, order: 1, width: 3 },
    { id: ids.uiGroupSimOverview, page: ids.uiPageSimulation, name: "[SIM] Daten", height: 14, order: 2 }
].forEach(({ id, page, name, height, order, width = 12 }) => {
    addNode({
        id,
        type: "ui-group",
        name,
        page,
        width,
        height,
        order,
        showTitle: false,
        className: "",
        visible: true,
        disabled: false,
        groupType: "default"
    });
});

[
    { nodeId: ids.mqttServerStatus, name: "Server Status", topic: "smarthome/server/status", y: 60 },
    { nodeId: ids.mqttMasterStatus, name: "Master Status", topic: "smarthome/master/+/status", y: 120 },
    { nodeId: ids.mqttMasterEvent, name: "Master Event", topic: "smarthome/master/+/event", y: 180 },
    { nodeId: ids.mqttNodeMeta, name: "Node Meta", topic: "smarthome/node/+/meta", y: 240 },
    { nodeId: ids.mqttNodeStatus, name: "Node Status", topic: "smarthome/node/+/status", y: 300 },
    { nodeId: ids.mqttNodeState, name: "Node State", topic: "smarthome/node/+/state", y: 360 },
    { nodeId: ids.mqttNodeEvent, name: "Node Event", topic: "smarthome/node/+/event", y: 420 },
    { nodeId: ids.mqttNodeAck, name: "Node Ack", topic: "smarthome/node/+/ack", y: 480 },
    { nodeId: ids.mqttNodeCfgReport, name: "Node Cfg Report", topic: "smarthome/node/+/cfg/report", y: 540 }
].forEach(({ nodeId, name, topic, y }) => {
    addNode({
        id: nodeId,
        type: "mqtt in",
        z: ids.tabIngest,
        name,
        topic,
        qos: "1",
        datatype: "auto-detect",
        broker: ids.mqttBroker,
        nl: false,
        rap: true,
        rh: 0,
        inputs: 0,
        x: 170,
        y,
        wires: [[ids.normalize]]
    });
});

addNode({
    id: ids.normalize,
    type: "function",
    z: ids.tabIngest,
    name: "Normalize MQTT",
    func: normalizeFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 430,
    y: 300,
    wires: [[ids.linkOutAudit, ids.routeCore]]
});

addNode({
    id: ids.routeCore,
    type: "function",
    z: ids.tabIngest,
    name: "Route Core Paths",
    func: routeCoreFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 690,
    y: 300,
    wires: [[ids.linkOutDeviceRegistry], [ids.linkOutServerStatus]]
});

addNode({
    id: ids.linkOutDeviceRegistry,
    type: "link out",
    z: ids.tabIngest,
    name: "To Registry",
    mode: "link",
    links: [ids.linkInDeviceRegistry],
    x: 915,
    y: 270,
    wires: []
});

addNode({
    id: ids.linkOutServerStatus,
    type: "link out",
    z: ids.tabIngest,
    name: "To Server Status Store",
    mode: "link",
    links: [ids.linkInServerStatus],
    x: 945,
    y: 330,
    wires: []
});

addNode({
    id: ids.linkOutAudit,
    type: "link out",
    z: ids.tabIngest,
    name: "To Audit",
    mode: "link",
    links: [ids.linkInAudit],
    x: 690,
    y: 120,
    wires: []
});

addNode({
    id: ids.linkInDeviceRegistry,
    type: "link in",
    z: ids.tabRegistry,
    name: "From Ingest",
    links: [ids.linkOutDeviceRegistry],
    x: 135,
    y: 140,
    wires: [[ids.buildDeviceUpsert]]
});

addNode({
    id: ids.buildDeviceUpsert,
    type: "function",
    z: ids.tabRegistry,
    name: "Build Device Upsert",
    func: buildDeviceUpsertFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 410,
    y: 140,
    wires: [[ids.sqliteDeviceUpsert]]
});

addNode({
    id: ids.sqliteDeviceUpsert,
    type: "sqlite",
    z: ids.tabRegistry,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Upsert Device",
    x: 670,
    y: 140,
    wires: [[ids.routePostRegistry]]
});

addNode({
    id: ids.routePostRegistry,
    type: "function",
    z: ids.tabRegistry,
    name: "Route Post Registry",
    func: routePostRegistryFunc,
    outputs: 5,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 930,
    y: 140,
    wires: [[ids.buildCapabilityUpserts], [ids.linkOutStateStore], [ids.linkOutConfigStore], [ids.linkOutTimeSeries], [ids.linkOutAutomationState]]
});

addNode({
    id: ids.linkOutTimeSeries,
    type: "link out",
    z: ids.tabRegistry,
    name: "To Time Series",
    mode: "link",
    links: [ids.linkInTimeSeries],
    x: 1225,
    y: 220,
    wires: []
});

addNode({
    id: ids.buildCapabilityUpserts,
    type: "function",
    z: ids.tabRegistry,
    name: "Build Capability Upserts",
    func: buildCapabilityUpsertsFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 1220,
    y: 100,
    wires: [[ids.sqliteCapabilityUpserts]]
});

addNode({
    id: ids.sqliteCapabilityUpserts,
    type: "sqlite",
    z: ids.tabRegistry,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Upsert Capabilities",
    x: 1490,
    y: 100,
    wires: [[]]
});

addNode({
    id: ids.linkOutStateStore,
    type: "link out",
    z: ids.tabRegistry,
    name: "To State Store",
    mode: "link",
    links: [ids.linkInStateStore],
    x: 1220,
    y: 140,
    wires: []
});

addNode({
    id: ids.linkOutConfigStore,
    type: "link out",
    z: ids.tabRegistry,
    name: "To Config Store",
    mode: "link",
    links: [ids.linkInConfigStore],
    x: 1230,
    y: 180,
    wires: []
});

addNode({
    id: ids.linkOutAutomationState,
    type: "link out",
    z: ids.tabRegistry,
    name: "To Automation Engine",
    mode: "link",
    links: [ids.linkInAutomationState],
    x: 1250,
    y: 260,
    wires: []
});

addNode({
    id: ids.linkInStateStore,
    type: "link in",
    z: ids.tabState,
    name: "From Registry",
    links: [ids.linkOutStateStore],
    x: 145,
    y: 120,
    wires: [[ids.buildLastStateUpsert]]
});

addNode({
    id: ids.buildLastStateUpsert,
    type: "function",
    z: ids.tabState,
    name: "Build Last State Upsert",
    func: buildLastStateUpsertFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 430,
    y: 120,
    wires: [[ids.sqliteLastStateUpsert]]
});

addNode({
    id: ids.sqliteLastStateUpsert,
    type: "sqlite",
    z: ids.tabState,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Upsert Last State",
    x: 730,
    y: 120,
    wires: [[]]
});

addNode({
    id: ids.linkInServerStatus,
    type: "link in",
    z: ids.tabState,
    name: "From Ingest",
    links: [ids.linkOutServerStatus],
    x: 145,
    y: 220,
    wires: [[ids.buildServerStatusUpserts]]
});

addNode({
    id: ids.buildServerStatusUpserts,
    type: "function",
    z: ids.tabState,
    name: "Build Server Status Upserts",
    func: buildServerStatusUpsertsFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 455,
    y: 220,
    wires: [[ids.sqliteServerStatusUpserts]]
});

addNode({
    id: ids.sqliteServerStatusUpserts,
    type: "sqlite",
    z: ids.tabState,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Upsert Server Status",
    x: 765,
    y: 220,
    wires: [[]]
});

addNode({
    id: ids.linkInConfigStore,
    type: "link in",
    z: ids.tabConfig,
    name: "From Registry",
    links: [ids.linkOutConfigStore],
    x: 145,
    y: 120,
    wires: [[ids.buildConfigUpserts]]
});

addNode({
    id: ids.buildConfigUpserts,
    type: "function",
    z: ids.tabConfig,
    name: "Build Config Upserts",
    func: buildConfigUpsertsFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 425,
    y: 120,
    wires: [[ids.sqliteConfigUpserts]]
});

addNode({
    id: ids.sqliteConfigUpserts,
    type: "sqlite",
    z: ids.tabConfig,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Upsert Device Config",
    x: 720,
    y: 120,
    wires: [[]]
});

addNode({
    id: ids.linkInTimeSeries,
    type: "link in",
    z: ids.tabTimeSeries,
    name: "From Registry",
    links: [ids.linkOutTimeSeries],
    x: 145,
    y: 120,
    wires: [[ids.buildInfluxLine]]
});

addNode({
    id: ids.buildInfluxLine,
    type: "function",
    z: ids.tabTimeSeries,
    name: "Build Influx Line",
    func: buildInfluxLineFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 420,
    y: 120,
    wires: [[ids.influxWrite]]
});

addNode({
    id: ids.influxWrite,
    type: "http request",
    z: ids.tabTimeSeries,
    name: "Write InfluxDB",
    method: "POST",
    ret: "txt",
    paytoqs: "ignore",
    url: "",
    persist: false,
    proxy: "",
    insecureHTTPParser: false,
    authType: "",
    senderr: true,
    headers: [],
    x: 710,
    y: 120,
    wires: [[]]
});

addNode({
    id: ids.injectSimDashboardLoad,
    type: "inject",
    z: ids.tabDashboardSim,
    name: "Load SIM Dashboard on Deploy",
    props: [
        { p: "payload" },
        { p: "topic", vt: "str" }
    ],
    repeat: "",
    crontab: "",
    once: true,
    onceDelay: 2,
    topic: "sim_dashboard_boot",
    payload: "true",
    payloadType: "bool",
    x: 230,
    y: 60,
    wires: [[ids.routeSimDashboardRefresh]]
});

addNode({
    id: ids.uiEventDashboard,
    type: "ui-event",
    z: ids.tabDashboardSim,
    ui: ids.uiBase,
    name: "Dashboard Events",
    x: 170,
    y: 120,
    wires: [[ids.routeSimDashboardRefresh]]
});

addNode({
    id: ids.uiButtonRefreshSim,
    type: "ui-button",
    z: ids.tabDashboardSim,
    group: ids.uiGroupSimControls,
    name: "SIM neu laden",
    label: "SIM neu laden",
    order: 1,
    width: 3,
    height: 1,
    emulateClick: false,
    tooltip: "",
    color: "",
    bgcolor: "",
    className: "",
    icon: "refresh",
    iconPosition: "left",
    payload: "true",
    payloadType: "bool",
    topic: "sim_dashboard_refresh",
    topicType: "str",
    buttonColor: "",
    textColor: "",
    iconColor: "",
    enableClick: true,
    enablePointerdown: false,
    pointerdownPayload: "",
    pointerdownPayloadType: "str",
    enablePointerup: false,
    pointerupPayload: "",
    pointerupPayloadType: "str",
    x: 180,
    y: 180,
    wires: [[ids.routeSimDashboardRefresh]]
});

addNode({
    id: ids.routeSimDashboardRefresh,
    type: "function",
    z: ids.tabDashboardSim,
    name: "Route SIM Dashboard Refresh",
    func: routeSimDashboardRefreshFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 470,
    y: 120,
    wires: [[ids.buildSimDashboardQuery]]
});

addNode({
    id: ids.buildSimDashboardQuery,
    type: "function",
    z: ids.tabDashboardSim,
    name: "Build SIM Dashboard Query",
    func: buildSimDashboardQueryFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 760,
    y: 120,
    wires: [[ids.sqliteSimDashboardQuery]]
});

addNode({
    id: ids.sqliteSimDashboardQuery,
    type: "sqlite",
    z: ids.tabDashboardSim,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Read SIM Dashboard Rows",
    x: 1030,
    y: 120,
    wires: [[ids.buildSimDashboardMarkdown]]
});

addNode({
    id: ids.buildSimDashboardMarkdown,
    type: "function",
    z: ids.tabDashboardSim,
    name: "Build SIM Dashboard Markdown",
    func: buildSimDashboardMarkdownFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 690,
    y: 220,
    wires: [[ids.uiTemplateSimOverview]]
});

addNode({
    id: ids.uiTemplateSimOverview,
    type: "ui-template",
    z: ids.tabDashboardSim,
    group: ids.uiGroupSimOverview,
    name: "[SIM] Sicht",
    order: 1,
    width: 12,
    height: 14,
    format: simDashboardTemplate,
    storeOutMessages: true,
    passthru: false,
    resendOnRefresh: true,
    templateScope: "local",
    className: "sim-dashboard-markdown",
    x: 1010,
    y: 220,
    wires: [[]]
});

addNode({
    id: ids.uiTemplateHeader,
    type: "ui-template",
    z: ids.tabDashboardUi,
    ui: ids.uiBase,
    name: "Global Header",
    order: 0,
    width: 0,
    height: 0,
    format: headerTemplate,
    storeOutMessages: true,
    passthru: false,
    resendOnRefresh: true,
    templateScope: "widget:ui",
    className: "",
    x: 250,
    y: 320,
    wires: [[ids.routeHeaderAction]]
});

[
    { id: ids.uiTemplateOverview, group: ids.uiGroupOverview, name: "Overview View", order: 1, height: 18, format: buildDeviceGridTemplate({ viewKey: "overview", title: "Übersicht", intro: "Kompakte Geräteübersicht mit Direktbedienung.", emptyText: "Noch keine Gerätekarten sichtbar. Master werden nur oben gezählt." }) },
    { id: ids.uiTemplateDevices, group: ids.uiGroupDevices, name: "Devices View", order: 1, height: 18, format: buildDeviceGridTemplate({ viewKey: "devices", title: "Geräte", intro: "Alle bekannten Geräte mit Detailzugriff.", emptyText: "Keine Geräte vorhanden." }) },
    { id: ids.uiTemplateSensors, group: ids.uiGroupSensors, name: "Sensors View", order: 1, height: 18, format: buildDeviceGridTemplate({ viewKey: "sensors", title: "Sensoren", intro: "Gefilterte Sicht auf Sensorgeräte und letzte Werte.", emptyText: "Keine Sensorgeräte erkannt." }) },
    { id: ids.uiTemplateActors, group: ids.uiGroupActors, name: "Actors View", order: 1, height: 18, format: buildDeviceGridTemplate({ viewKey: "actors", title: "Aktoren", intro: "Schaltbare Geräte mit direktem cmd/set-Pfad.", emptyText: "Keine schaltbaren Aktoren erkannt." }) },
    { id: ids.uiTemplateCovers, group: ids.uiGroupCovers, name: "Covers View", order: 1, height: 18, format: buildDeviceGridTemplate({ viewKey: "covers", title: "Rollläden", intro: "Rollladenansicht für Geräte im echten Rollladenmodus.", emptyText: "Keine Rollläden sichtbar." }) }
].forEach(({ id, group, name, order, height, format }) => {
    addNode({
        id,
        type: "ui-template",
        z: ids.tabDashboardUi,
        group,
        name,
        order,
        width: 12,
        height,
        format,
        storeOutMessages: true,
        passthru: false,
        resendOnRefresh: true,
        templateScope: "local",
        className: "",
        x: 250,
        y: 420,
        wires: [[ids.routeDeviceDatasetAction]]
    });
});

addNode({
    id: ids.uiTemplateRooms,
    type: "ui-template",
    z: ids.tabDashboardUi,
    group: ids.uiGroupRooms,
    name: "Rooms View",
    order: 1,
    width: 12,
    height: 14,
    format: roomsTemplate,
    storeOutMessages: true,
    passthru: false,
    resendOnRefresh: true,
    templateScope: "local",
    className: "",
    x: 250,
    y: 500,
    wires: [[ids.routeDeviceDatasetAction]]
});

addNode({
    id: ids.uiTemplateCharts,
    type: "ui-template",
    z: ids.tabDashboardUi,
    group: ids.uiGroupCharts,
    name: "Charts View",
    order: 1,
    width: 12,
    height: 16,
    format: chartsTemplate,
    storeOutMessages: true,
    passthru: false,
    resendOnRefresh: true,
    templateScope: "local",
    className: "",
    x: 250,
    y: 580,
    wires: [[ids.routeDeviceDatasetAction]]
});

addNode({
    id: ids.uiTemplateDeviceDetail,
    type: "ui-template",
    z: ids.tabDashboardUi,
    group: ids.uiGroupDeviceDetail,
    name: "Device Detail View",
    order: 1,
    width: 12,
    height: 24,
    format: deviceDetailTemplate,
    storeOutMessages: true,
    passthru: false,
    resendOnRefresh: true,
    templateScope: "local",
    className: "",
    x: 250,
    y: 660,
    wires: [[ids.routeDetailAction]]
});

addNode({
    id: ids.uiTemplateWeather,
    type: "ui-template",
    z: ids.tabDashboardUi,
    group: ids.uiGroupWeather,
    name: "Weather View",
    order: 1,
    width: 12,
    height: 12,
    format: weatherTemplate,
    storeOutMessages: true,
    passthru: false,
    resendOnRefresh: true,
    templateScope: "local",
    className: "",
    x: 250,
    y: 740,
    wires: [[ids.routeWeatherAction]]
});

addNode({
    id: ids.uiTemplateAutomations,
    type: "ui-template",
    z: ids.tabDashboardUi,
    group: ids.uiGroupAutomations,
    name: "Automation View",
    order: 1,
    width: 12,
    height: 18,
    format: automationsTemplate,
    storeOutMessages: true,
    passthru: false,
    resendOnRefresh: true,
    templateScope: "local",
    className: "",
    x: 250,
    y: 820,
    wires: [[ids.routeAutomationAction]]
});

addNode({
    id: ids.uiTemplateConfig,
    type: "ui-template",
    z: ids.tabDashboardUi,
    group: ids.uiGroupConfig,
    name: "Config View",
    order: 1,
    width: 12,
    height: 18,
    format: configTemplate,
    storeOutMessages: true,
    passthru: false,
    resendOnRefresh: true,
    templateScope: "local",
    className: "",
    x: 250,
    y: 900,
    wires: [[ids.routeConfigAction]]
});

addNode({
    id: ids.uiTemplateLogs,
    type: "ui-template",
    z: ids.tabDashboardUi,
    group: ids.uiGroupLogs,
    name: "Logs View",
    order: 1,
    width: 12,
    height: 18,
    format: logsTemplate,
    storeOutMessages: true,
    passthru: false,
    resendOnRefresh: true,
    templateScope: "local",
    className: "",
    x: 250,
    y: 980,
    wires: [[ids.routeLogsAction]]
});

addNode({
    id: ids.routeDeviceDatasetAction,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Device Dataset Action",
    func: routeDeviceDatasetActionFunc,
    outputs: 3,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 590,
    y: 500,
    wires: [[ids.buildDeviceDatasetQuery], [ids.buildGenericCommand], [ids.buildChartRequest]]
});

addNode({
    id: ids.buildDeviceDatasetQuery,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Device Dataset Query",
    func: buildDeviceDatasetQueryFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 900,
    y: 420,
    wires: [[ids.sqliteDeviceDatasetQuery]]
});

addNode({
    id: ids.sqliteDeviceDatasetQuery,
    type: "sqlite",
    z: ids.tabDashboardUi,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Read Device Dataset",
    x: 1180,
    y: 420,
    wires: [[ids.buildDeviceDatasetPayload]]
});

addNode({
    id: ids.buildDeviceDatasetPayload,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Device Dataset Payload",
    func: buildDeviceDatasetPayloadFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 930,
    y: 520,
    wires: [[ids.routeDeviceDatasetPayload]]
});

addNode({
    id: ids.routeDeviceDatasetPayload,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Device Dataset Payload",
    func: routeDeviceDatasetPayloadFunc,
    outputs: 7,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 1210,
    y: 520,
    wires: [[ids.uiTemplateOverview], [ids.uiTemplateRooms], [ids.uiTemplateDevices], [ids.uiTemplateSensors], [ids.uiTemplateActors], [ids.uiTemplateCovers], [ids.uiTemplateCharts]]
});

addNode({
    id: ids.routeDetailAction,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Detail Action",
    func: routeDetailActionFunc,
    outputs: 5,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 560,
    y: 660,
    wires: [[ids.buildDeviceDetailQuery], [ids.buildDeviceSaveStatements], [ids.buildDeviceDeleteStatements], [ids.buildChartRequest], [ids.buildGenericCommand]]
});

addNode({
    id: ids.buildDeviceDetailQuery,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Device Detail Query",
    func: buildDeviceDetailQueryFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 870,
    y: 620,
    wires: [[ids.sqliteDeviceDetailQuery]]
});

addNode({
    id: ids.sqliteDeviceDetailQuery,
    type: "sqlite",
    z: ids.tabDashboardUi,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Read Device Detail",
    x: 1140,
    y: 620,
    wires: [[ids.buildDeviceDetailPayload]]
});

addNode({
    id: ids.buildDeviceDetailPayload,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Device Detail Payload",
    func: buildDeviceDetailPayloadFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 900,
    y: 700,
    wires: [[ids.uiTemplateDeviceDetail]]
});

addNode({
    id: ids.buildDeviceSaveStatements,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Device Save Statements",
    func: buildDeviceSaveStatementsFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 900,
    y: 760,
    wires: [[ids.sqliteDeviceDetailWrite]]
});

addNode({
    id: ids.buildDeviceDeleteStatements,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Device Delete Statements",
    func: buildDeviceDeleteStatementsFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 900,
    y: 820,
    wires: [[ids.sqliteDeviceDetailWrite]]
});

addNode({
    id: ids.sqliteDeviceDetailWrite,
    type: "sqlite",
    z: ids.tabDashboardUi,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Write Device Detail",
    x: 1180,
    y: 790,
    wires: [[ids.routePostDeviceDetailWrite]]
});

addNode({
    id: ids.routePostDeviceDetailWrite,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Post Device Detail Write",
    func: routePostDeviceDetailWriteFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 910,
    y: 900,
    wires: [[ids.uiTemplateDeviceDetail], [ids.buildDeviceDetailQuery]]
});

addNode({
    id: ids.routeWeatherAction,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Weather Action",
    func: routeWeatherActionFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 570,
    y: 1060,
    wires: [[ids.buildWeatherQuery], [ids.buildWeatherSaveStatements]]
});

addNode({
    id: ids.buildWeatherQuery,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Weather Query",
    func: buildWeatherQueryFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 860,
    y: 1020,
    wires: [[ids.sqliteWeatherQuery]]
});

addNode({
    id: ids.sqliteWeatherQuery,
    type: "sqlite",
    z: ids.tabDashboardUi,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Read Weather Settings",
    x: 1130,
    y: 1020,
    wires: [[ids.buildWeatherPayload]]
});

addNode({
    id: ids.buildWeatherPayload,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Weather Payload",
    func: buildWeatherPayloadFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 890,
    y: 1100,
    wires: [[ids.uiTemplateWeather]]
});

addNode({
    id: ids.buildWeatherSaveStatements,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Weather Save Statements",
    func: buildWeatherSaveStatementsFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 890,
    y: 1160,
    wires: [[ids.sqliteWeatherWrite]]
});

addNode({
    id: ids.sqliteWeatherWrite,
    type: "sqlite",
    z: ids.tabDashboardUi,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Write Weather Settings",
    x: 1170,
    y: 1160,
    wires: [[ids.routePostWeatherWrite]]
});

addNode({
    id: ids.routePostWeatherWrite,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Post Weather Write",
    func: routePostWeatherWriteFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 905,
    y: 1230,
    wires: [[ids.buildWeatherQuery, ids.buildHeaderQuery]]
});

addNode({
    id: ids.routeHeaderAction,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Header Action",
    func: routeHeaderActionFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 560,
    y: 1280,
    wires: [[ids.buildHeaderQuery]]
});

addNode({
    id: ids.buildHeaderQuery,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Header Query",
    func: buildHeaderQueryFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 855,
    y: 1280,
    wires: [[ids.sqliteHeaderQuery]]
});

addNode({
    id: ids.sqliteHeaderQuery,
    type: "sqlite",
    z: ids.tabDashboardUi,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Read Header Data",
    x: 1125,
    y: 1280,
    wires: [[ids.buildHeaderPayload]]
});

addNode({
    id: ids.buildHeaderPayload,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Header Payload",
    func: buildHeaderPayloadFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 890,
    y: 1360,
    wires: [[ids.uiTemplateHeader]]
});

addNode({
    id: ids.routeAutomationAction,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Automation Action",
    func: routeAutomationActionFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 580,
    y: 1320,
    wires: [[ids.buildAutomationQuery], [ids.buildAutomationWriteStatements]]
});

addNode({
    id: ids.buildAutomationQuery,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Automation Query",
    func: buildAutomationQueryFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 870,
    y: 1280,
    wires: [[ids.sqliteAutomationQuery]]
});

addNode({
    id: ids.sqliteAutomationQuery,
    type: "sqlite",
    z: ids.tabDashboardUi,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Read Automations",
    x: 1130,
    y: 1280,
    wires: [[ids.buildAutomationPayload]]
});

addNode({
    id: ids.buildAutomationPayload,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Automation Payload",
    func: buildAutomationPayloadFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 900,
    y: 1360,
    wires: [[ids.uiTemplateAutomations]]
});

addNode({
    id: ids.buildAutomationWriteStatements,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Automation Write Statements",
    func: buildAutomationWriteStatementsFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 910,
    y: 1420,
    wires: [[ids.sqliteAutomationWrite]]
});

addNode({
    id: ids.sqliteAutomationWrite,
    type: "sqlite",
    z: ids.tabDashboardUi,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Write Automation",
    x: 1180,
    y: 1420,
    wires: [[ids.routePostAutomationWrite]]
});

addNode({
    id: ids.routePostAutomationWrite,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Post Automation Write",
    func: routePostAutomationWriteFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 920,
    y: 1490,
    wires: [[ids.buildAutomationQuery]]
});

addNode({
    id: ids.routeConfigAction,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Config Action",
    func: routeConfigActionFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 560,
    y: 1560,
    wires: [[ids.buildConfigPageQuery]]
});

addNode({
    id: ids.buildConfigPageQuery,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Config Page Query",
    func: buildConfigPageQueryFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 870,
    y: 1560,
    wires: [[ids.sqliteConfigPageQuery]]
});

addNode({
    id: ids.sqliteConfigPageQuery,
    type: "sqlite",
    z: ids.tabDashboardUi,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Read Config Page Data",
    x: 1140,
    y: 1560,
    wires: [[ids.buildConfigPagePayload]]
});

addNode({
    id: ids.buildConfigPagePayload,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Config Page Payload",
    func: buildConfigPagePayloadFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 900,
    y: 1630,
    wires: [[ids.uiTemplateConfig]]
});

addNode({
    id: ids.routeLogsAction,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Logs Action",
    func: routeLogsActionFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 560,
    y: 1700,
    wires: [[ids.buildLogsQuery]]
});

addNode({
    id: ids.buildLogsQuery,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Logs Query",
    func: buildLogsQueryFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 840,
    y: 1700,
    wires: [[ids.sqliteLogsQuery]]
});

addNode({
    id: ids.sqliteLogsQuery,
    type: "sqlite",
    z: ids.tabDashboardUi,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Read Logs",
    x: 1090,
    y: 1700,
    wires: [[ids.buildLogsPayload]]
});

addNode({
    id: ids.buildLogsPayload,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Logs Payload",
    func: buildLogsPayloadFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 860,
    y: 1770,
    wires: [[ids.uiTemplateLogs]]
});

addNode({
    id: ids.buildChartRequest,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Build Chart Request",
    func: buildChartRequestFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 875,
    y: 1850,
    wires: [[ids.influxChartQuery], [ids.routeChartPayload]]
});

addNode({
    id: ids.influxChartQuery,
    type: "http request",
    z: ids.tabDashboardUi,
    name: "Influx Chart Query",
    method: "use",
    ret: "txt",
    paytoqs: "ignore",
    url: "",
    persist: false,
    proxy: "",
    insecureHTTPParser: false,
    authType: "",
    senderr: true,
    headers: [],
    x: 1150,
    y: 1850,
    wires: [[ids.parseChartResponse]]
});

addNode({
    id: ids.parseChartResponse,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Parse Chart Response",
    func: parseChartResponseFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 890,
    y: 1930,
    wires: [[ids.routeChartPayload]]
});

addNode({
    id: ids.routeChartPayload,
    type: "function",
    z: ids.tabDashboardUi,
    name: "Route Chart Payload",
    func: routeChartPayloadFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 1160,
    y: 1930,
    wires: [[ids.uiTemplateDeviceDetail], [ids.uiTemplateCharts]]
});

addNode({
    id: ids.linkInAutomationState,
    type: "link in",
    z: ids.tabAutomation,
    name: "From State Updates",
    links: [ids.linkOutAutomationState],
    x: 150,
    y: 80,
    wires: [[ids.buildSensorAutomationQuery]]
});

addNode({
    id: ids.buildSensorAutomationQuery,
    type: "function",
    z: ids.tabAutomation,
    name: "Build Sensor Automation Query",
    func: buildSensorAutomationQueryFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 450,
    y: 80,
    wires: [[ids.sqliteSensorAutomationQuery]]
});

addNode({
    id: ids.sqliteSensorAutomationQuery,
    type: "sqlite",
    z: ids.tabAutomation,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Read Sensor Automations",
    x: 730,
    y: 80,
    wires: [[ids.evaluateSensorAutomations]]
});

addNode({
    id: ids.evaluateSensorAutomations,
    type: "function",
    z: ids.tabAutomation,
    name: "Evaluate Sensor Automations",
    func: evaluateSensorAutomationsFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 470,
    y: 160,
    wires: [[ids.buildGenericCommand], [ids.buildAutomationExecutionUpdate]]
});

addNode({
    id: ids.injectAutomationTimeTick,
    type: "inject",
    z: ids.tabAutomation,
    name: "Automation Minute Tick",
    props: [
        { p: "payload" }
    ],
    repeat: "60",
    crontab: "",
    once: true,
    onceDelay: 3,
    payload: "true",
    payloadType: "bool",
    x: 180,
    y: 260,
    wires: [[ids.buildTimeAutomationQuery]]
});

addNode({
    id: ids.buildTimeAutomationQuery,
    type: "function",
    z: ids.tabAutomation,
    name: "Build Time Automation Query",
    func: buildTimeAutomationQueryFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 450,
    y: 260,
    wires: [[ids.sqliteTimeAutomationQuery]]
});

addNode({
    id: ids.sqliteTimeAutomationQuery,
    type: "sqlite",
    z: ids.tabAutomation,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Read Time Automations",
    x: 720,
    y: 260,
    wires: [[ids.evaluateTimeAutomations]]
});

addNode({
    id: ids.evaluateTimeAutomations,
    type: "function",
    z: ids.tabAutomation,
    name: "Evaluate Time Automations",
    func: evaluateTimeAutomationsFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 460,
    y: 340,
    wires: [[ids.buildGenericCommand], [ids.buildAutomationExecutionUpdate]]
});

addNode({
    id: ids.buildAutomationExecutionUpdate,
    type: "function",
    z: ids.tabAutomation,
    name: "Build Automation Execution Update",
    func: buildAutomationExecutionUpdateFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 490,
    y: 430,
    wires: [[ids.sqliteAutomationExecutionUpdate]]
});

addNode({
    id: ids.sqliteAutomationExecutionUpdate,
    type: "sqlite",
    z: ids.tabAutomation,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Update Automation Execution",
    x: 780,
    y: 430,
    wires: [[]]
});

[
    { nodeId: ids.injectNetErlOn, name: "Trigger Relay 1 EIN", payload: "true", x: 180, y: 60 },
    { nodeId: ids.injectNetErlOff, name: "Trigger Relay 1 AUS", payload: "false", x: 180, y: 240 }
].forEach(({ nodeId, name, payload, x, y }) => {
    addNode({
        id: nodeId,
        type: "inject",
        z: ids.tabCommand,
        name,
        props: [
            { p: "payload" },
            { p: "topic", vt: "str" }
        ],
        repeat: "",
        crontab: "",
        once: false,
        onceDelay: 0.1,
        topic: "",
        payload,
        payloadType: "bool",
        x,
        y,
        wires: [[ids.buildNetErlCmdSet]]
    });
});

[
    { nodeId: ids.injectNetZrlRelay1On, name: "Trigger net_zrl Relay 1 EIN", topic: "relay_1", payload: "true", x: 190, y: 360 },
    { nodeId: ids.injectNetZrlRelay1Off, name: "Trigger net_zrl Relay 1 AUS", topic: "relay_1", payload: "false", x: 190, y: 420 },
    { nodeId: ids.injectNetZrlRelay2On, name: "Trigger net_zrl Relay 2 EIN", topic: "relay_2", payload: "true", x: 190, y: 540 },
    { nodeId: ids.injectNetZrlRelay2Off, name: "Trigger net_zrl Relay 2 AUS", topic: "relay_2", payload: "false", x: 190, y: 600 }
].forEach(({ nodeId, name, topic, payload, x, y }) => {
    addNode({
        id: nodeId,
        type: "inject",
        z: ids.tabCommand,
        name,
        props: [
            { p: "payload" },
            { p: "topic", vt: "str" }
        ],
        repeat: "",
        crontab: "",
        once: false,
        onceDelay: 0.1,
        topic,
        payload,
        payloadType: "bool",
        x,
        y,
        wires: [[ids.buildNetZrlCmdSet]]
    });
});

addNode({
    id: ids.buildNetErlCmdSet,
    type: "function",
    z: ids.tabCommand,
    name: "Build net_erl_01 cmd/set",
    func: buildNetErlCmdSetFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 510,
    y: 150,
    wires: [[ids.mqttCmdSetOut], [ids.linkOutAuditEgress]]
});

addNode({
    id: ids.buildNetZrlCmdSet,
    type: "function",
    z: ids.tabCommand,
    name: "Build net_zrl_01 cmd/set",
    func: buildNetZrlCmdSetFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 530,
    y: 510,
    wires: [[ids.mqttCmdSetOut], [ids.linkOutAuditEgress]]
});

addNode({
    id: ids.buildGenericCommand,
    type: "function",
    z: ids.tabCommand,
    name: "Build Generic cmd/set",
    func: buildGenericCommandFunc,
    outputs: 2,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 520,
    y: 900,
    wires: [[ids.mqttCmdSetOut], [ids.linkOutAuditEgress]]
});

addNode({
    id: ids.mqttCmdSetOut,
    type: "mqtt out",
    z: ids.tabCommand,
    name: "Publish cmd/set",
    topic: "",
    qos: "1",
    retain: "false",
    respTopic: "",
    contentType: "",
    userProps: "",
    correl: "",
    expiry: "",
    broker: ids.mqttBroker,
    x: 810,
    y: 150,
    wires: []
});

addNode({
    id: ids.linkOutAuditEgress,
    type: "link out",
    z: ids.tabCommand,
    name: "To Audit",
    mode: "link",
    links: [ids.linkInAudit],
    x: 825,
    y: 210,
    wires: []
});

addNode({
    id: ids.linkInAudit,
    type: "link in",
    z: ids.tabLogging,
    name: "From Ingest/Egress",
    links: [ids.linkOutAudit, ids.linkOutAuditEgress],
    x: 135,
    y: 100,
    wires: [[ids.buildAuditInsert]]
});

addNode({
    id: ids.buildAuditInsert,
    type: "function",
    z: ids.tabLogging,
    name: "Build Audit Insert",
    func: buildAuditInsertFunc,
    outputs: 1,
    noerr: 0,
    initialize: "",
    finalize: "",
    libs: [],
    x: 420,
    y: 100,
    wires: [[ids.sqliteAuditInsert]]
});

addNode({
    id: ids.sqliteAuditInsert,
    type: "sqlite",
    z: ids.tabLogging,
    mydb: ids.sqliteDb,
    sqlquery: "msg.topic",
    sql: "",
    name: "Insert Audit Row",
    x: 710,
    y: 100,
    wires: [[]]
});

process.stdout.write(`${JSON.stringify(nodes, null, 2)}\n`);
