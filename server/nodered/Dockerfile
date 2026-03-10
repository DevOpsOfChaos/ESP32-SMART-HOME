FROM nodered/node-red:latest

USER root

COPY nodered/install-deps.sh /tmp/install-deps.sh
RUN chmod +x /tmp/install-deps.sh && /tmp/install-deps.sh && rm /tmp/install-deps.sh

COPY nodered/package.json /data/package.json
RUN cd /data && npm install --unsafe-perm --no-update-notifier --no-fund --omit=dev

COPY nodered/settings.js /data/settings.js
COPY nodered/build-flows.js /opt/smarthome/build-flows.js
COPY db/schema.sql /opt/smarthome/schema.sql
RUN node /opt/smarthome/build-flows.js > /data/flows.json
RUN chown -R node-red:root /data

USER node-red
