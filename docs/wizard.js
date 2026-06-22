  /* ── Language ── */
  function toggleLang() {
    const body = document.body;
    const toDE = body.classList.contains('lang-en');
    body.classList.toggle('lang-de', toDE);
    body.classList.toggle('lang-en', !toDE);
    // Re-render wizard in new language using answer count, not element style
    const lang = toDE ? 'de' : 'en';
    if (wizAnswers.length >= QUESTIONS[lang].length) {
      showResult();
    } else {
      renderQuestion();
    }
  }

  /* ── Nav Drawer ── */
  function toggleDrawer() {
    const drawer = document.getElementById('nav-drawer');
    const burger = document.getElementById('nav-hamburger');
    const isOpen = drawer.classList.toggle('open');
    burger.classList.toggle('open', isOpen);
    burger.setAttribute('aria-expanded', isOpen);
    document.body.style.overflow = isOpen ? 'hidden' : '';
  }
  function closeDrawer() {
    document.getElementById('nav-drawer').classList.remove('open');
    const b = document.getElementById('nav-hamburger');
    b.classList.remove('open'); b.setAttribute('aria-expanded','false');
    document.body.style.overflow = '';
  }
  document.addEventListener('click', e => {
    const drawer = document.getElementById('nav-drawer');
    if (drawer.classList.contains('open') &&
        !drawer.contains(e.target) && !document.getElementById('nav-hamburger').contains(e.target)) closeDrawer();
  });
  window.addEventListener('resize', () => { if (window.innerWidth > 720) closeDrawer(); });

  /* ── Wizard ── */
  const QUESTIONS = {
    de: [
      { q: "Nutzt du Home Assistant oder planst du es?", opts: [
        { icon:"🏠", label:"Ja, ich nutze HA aktiv", desc:"Home Assistant ist meine zentrale Steuerung", value:"ha_yes" },
        { icon:"🤔", label:"Vielleicht später", desc:"Ich bin noch offen", value:"ha_maybe" },
        { icon:"🔧", label:"Nein, kein HA geplant", desc:"Ich möchte einen eigenen lokalen Stack", value:"ha_no" },
      ]},
      { q: "Wie ist deine Erfahrung mit Embedded-Entwicklung?", opts: [
        { icon:"🚀", label:"Erfahren (C++, PlatformIO)", desc:"Ich kenne mich mit Microcontroller-Entwicklung aus", value:"exp_high" },
        { icon:"📚", label:"Mittel (Hobby-Projekte)", desc:"Ich habe schon Projekte umgesetzt, bin aber kein Profi", value:"exp_mid" },
        { icon:"🌱", label:"Einsteiger / YAML bevorzugt", desc:"Ich möchte nichts kompilieren müssen", value:"exp_low" },
      ]},
      { q: "Wie viel Kontrolle über das System willst du haben?", opts: [
        { icon:"⚡", label:"Volle Kontrolle", desc:"Protokoll, Payload und Transport selbst bestimmen", value:"ctrl_full" },
        { icon:"⚖️", label:"Gute Balance", desc:"Zuverlässig laufen ohne alles selbst bauen zu müssen", value:"ctrl_mid" },
        { icon:"🎯", label:"Einfachheit vor Kontrolle", desc:"Geräte schnell einbinden, kein tiefer Einstieg", value:"ctrl_low" },
      ]},
      { q: "Was planst du mit der Hardware?", opts: [
        { icon:"🔨", label:"Eigene PCBs bauen / anpassen", desc:"Hardware nach Repo-Design selbst fertigen", value:"hw_build" },
        { icon:"🛒", label:"Kompatible Module kaufen & flashen", desc:"Fertige ESP32 Module nehmen und flashen", value:"hw_flash" },
        { icon:"🔍", label:"Nur studieren / verstehen", desc:"System verstehen, noch nichts bauen", value:"hw_study" },
      ]},
    ],
    en: [
      { q: "Do you use Home Assistant or plan to?", opts: [
        { icon:"🏠", label:"Yes, I actively use HA", desc:"Home Assistant is my central hub", value:"ha_yes" },
        { icon:"🤔", label:"Maybe later", desc:"I'm still open to it", value:"ha_maybe" },
        { icon:"🔧", label:"No HA planned", desc:"I want my own local stack", value:"ha_no" },
      ]},
      { q: "What's your embedded development experience?", opts: [
        { icon:"🚀", label:"Experienced (C++, PlatformIO)", desc:"I know my way around microcontroller development", value:"exp_high" },
        { icon:"📚", label:"Intermediate (hobby projects)", desc:"I've done projects but not a professional", value:"exp_mid" },
        { icon:"🌱", label:"Beginner / prefer YAML", desc:"I don't want to compile anything", value:"exp_low" },
      ]},
      { q: "How much control do you want over the system?", opts: [
        { icon:"⚡", label:"Full control", desc:"Define protocol, payload and transport myself", value:"ctrl_full" },
        { icon:"⚖️", label:"Good balance", desc:"Reliable without building everything myself", value:"ctrl_mid" },
        { icon:"🎯", label:"Simplicity over control", desc:"Add devices quickly, no deep dive", value:"ctrl_low" },
      ]},
      { q: "What do you plan to do with the hardware?", opts: [
        { icon:"🔨", label:"Build / customize own PCBs", desc:"Manufacture hardware based on the repo design", value:"hw_build" },
        { icon:"🛒", label:"Buy compatible modules & flash", desc:"Use off-the-shelf ESP32 modules", value:"hw_flash" },
        { icon:"🔍", label:"Just study / understand", desc:"Understand the system, not build yet", value:"hw_study" },
      ]},
    ],
  };

  const RESULTS = {
    de: {
      custom: { icon:"⚙️", track:"Custom Firmware Track", cls:"custom",
        summary:"Du willst die volle Kontrolle. Der Custom Firmware Track mit PlatformIO, ESP-NOW und einem lokalen Server-Stack ist genau richtig.",
        steps:["firmware/README.md lesen – Architektur-Überblick","Secrets.h anlegen (WIFI + MQTT)","Server starten: docker compose up -d","Master flashen: pio run -e master_firmware --target upload","Gerät flashen und Setup-Portal durchlaufen"] },
      esphome: { icon:"🏠", track:"ESPHome Track", cls:"esphome",
        summary:"Du willst einfache Home Assistant Integration. Der ESPHome Track bietet genau das – ohne C++ oder ESP-NOW.",
        steps:["esphome/README.md lesen","Passendes YAML aus esphome/devices/ kopieren","secrets.yaml befüllen (WLAN + OTA-Passwort)","Config validieren: .\\scripts\\check_esphome.ps1","Per USB flashen, dann in HA adoptieren"] },
      both: { icon:"🔍", track:"Beide Tracks erkunden", cls:"both",
        summary:"Schau dir beide Tracks an. Die Architektur ist gut dokumentiert und du kannst beide unabhängig voneinander studieren.",
        steps:["docs/GETTING_STARTED.md lesen","README.md im Root für Überblick","Dann einen Track wählen und tief einsteigen"] },
    },
    en: {
      custom: { icon:"⚙️", track:"Custom Firmware Track", cls:"custom",
        summary:"You want full control. The custom firmware track with PlatformIO, ESP-NOW and a local server stack is exactly right.",
        steps:["Read firmware/README.md – architecture overview","Create Secrets.h (WiFi + MQTT)","Start server: docker compose up -d","Flash master: pio run -e master_firmware --target upload","Flash device and run through setup portal"] },
      esphome: { icon:"🏠", track:"ESPHome Track", cls:"esphome",
        summary:"You want simple Home Assistant integration. The ESPHome track gives you exactly that – without C++ or ESP-NOW.",
        steps:["Read esphome/README.md","Copy matching YAML from esphome/devices/","Fill secrets.yaml (WiFi + OTA password)","Validate config: .\\scripts\\check_esphome.ps1","Flash via USB, then adopt in HA"] },
      both: { icon:"🔍", track:"Explore Both Tracks", cls:"both",
        summary:"Look at both tracks. The architecture is well documented and you can study them independently.",
        steps:["Read docs/GETTING_STARTED.md","Read root README.md for overview","Then pick one track and dive deep"] },
    },
  };

  let wizAnswers = [], wizStep = 0;
  function getLang() { return document.body.classList.contains('lang-de') ? 'de' : 'en'; }

  function renderProgress() {
    const lang = getLang(), q = QUESTIONS[lang];
    document.getElementById('wiz-progress').innerHTML = q.map((_,i) => {
      let c = 'progress-dot';
      if (i < wizStep) c += ' done'; else if (i === wizStep) c += ' active';
      return `<div class="${c}"></div>`;
    }).join('');
  }

  function renderQuestion() {
    const lang = getLang(), q = QUESTIONS[lang][wizStep];
    document.getElementById('wiz-q').textContent = q.q;
    document.getElementById('wiz-counter').textContent = lang==='de'
      ? `Frage ${wizStep+1} von ${QUESTIONS[lang].length}`
      : `Question ${wizStep+1} of ${QUESTIONS[lang].length}`;
    document.getElementById('wiz-opts').innerHTML = q.opts.map((o,i) =>
      `<div class="wizard-opt${wizAnswers[wizStep]===o.value?' selected':''}" onclick="wizSelect('${o.value}',${i})">
        <span class="opt-icon">${o.icon}</span>
        <div><div class="opt-label">${o.label}</div><div class="opt-desc">${o.desc}</div></div>
      </div>`
    ).join('');
    document.getElementById('wiz-back').disabled = wizStep === 0;
    renderProgress();
  }

  function wizSelect(value, idx) {
    wizAnswers[wizStep] = value;
    document.querySelectorAll('.wizard-opt').forEach((el,i) => el.classList.toggle('selected', i===idx));
    setTimeout(() => {
      if (wizStep < QUESTIONS[getLang()].length - 1) { wizStep++; renderQuestion(); }
      else showResult();
    }, 340);
  }

  function wizBack() { if (wizStep>0) { wizStep--; renderQuestion(); } }

  function calcResult() {
    const a = wizAnswers;
    let c=0, e=0;
    if (a[0]==='ha_yes') e+=3; if (a[0]==='ha_maybe'){e++;c++;} if (a[0]==='ha_no') c+=3;
    if (a[1]==='exp_high') c+=3; if (a[1]==='exp_mid'){c++;e++;} if (a[1]==='exp_low') e+=3;
    if (a[2]==='ctrl_full') c+=3; if (a[2]==='ctrl_mid'){c++;e++;} if (a[2]==='ctrl_low') e+=3;
    if (a[3]==='hw_build') c+=2; if (a[3]==='hw_flash') e+=1; if (a[3]==='hw_study') return 'both';
    if (Math.abs(c-e)<=2) return 'both';
    return c>e ? 'custom' : 'esphome';
  }

  function buildReasons(track) {
    const lang = getLang(), a = wizAnswers, r = [];
    if (lang==='de') {
      if (track==='custom') {
        if (a[0]==='ha_no') r.push("Kein Home Assistant geplant → kein Vorteil durch ESPHome");
        if (a[1]==='exp_high') r.push("C++/PlatformIO-Erfahrung wird im Custom Track voll genutzt");
        if (a[2]==='ctrl_full') r.push("Volle Kontrolle über Protokoll, Payload und Transport");
        if (a[3]==='hw_build') r.push("Eigene PCBs profitieren vom vollständigen Hardware-Referenzdesign");
      } else if (track==='esphome') {
        if (a[0]==='ha_yes') r.push("Du nutzt HA aktiv → ESPHome ist der native, direkte Weg");
        if (a[1]==='exp_low') r.push("YAML-Konfiguration statt C++ kompilieren");
        if (a[2]==='ctrl_low') r.push("Einfache Geräteintegration ohne Protokoll-Einstieg");
        if (a[3]==='hw_flash') r.push("Fertige Module kaufen und direkt flashen");
      } else {
        r.push("Deine Antworten zeigen Interesse an beiden Aspekten");
        r.push("Beide Tracks sind gut dokumentiert und unabhängig studierbar");
        r.push("Du kannst jederzeit wechseln – Hardware bleibt dieselbe");
      }
    } else {
      if (track==='custom') {
        if (a[0]==='ha_no') r.push("No HA planned → no advantage from ESPHome");
        if (a[1]==='exp_high') r.push("C++/PlatformIO experience is fully used in the custom track");
        if (a[2]==='ctrl_full') r.push("Full control over protocol, payload and transport");
        if (a[3]==='hw_build') r.push("Custom PCBs benefit from the full hardware reference design");
      } else if (track==='esphome') {
        if (a[0]==='ha_yes') r.push("You actively use HA → ESPHome is the native, direct path");
        if (a[1]==='exp_low') r.push("YAML configuration instead of compiling C++");
        if (a[2]==='ctrl_low') r.push("Simple device integration without protocol knowledge");
        if (a[3]==='hw_flash') r.push("Buy ready modules and flash them directly");
      } else {
        r.push("Your answers show interest in both aspects");
        r.push("Both tracks are well documented and independently studyable");
        r.push("You can switch at any time – hardware stays the same");
      }
    }
    if (!r.length) r.push(lang==='de' ? "Basierend auf deinen Antworten ist dies die beste Empfehlung" : "Based on your answers, this is the best recommendation");
    return r;
  }

  function showResult() {
    const lang = getLang(), track = calcResult();
    const res = RESULTS[lang][track], reasons = buildReasons(track);
    document.getElementById('wiz-questions').style.display = 'none';
    const el = document.getElementById('wiz-result'); el.style.display = 'block';
    document.getElementById('res-icon').textContent = res.icon;
    const te = document.getElementById('res-track'); te.textContent = res.track; te.className = `result-track ${res.cls}`;
    document.getElementById('res-summary').textContent = res.summary;
    document.getElementById('res-reasons').innerHTML = reasons.map(r=>`<div class="reason-item"><span class="reason-check">✓</span><span>${r}</span></div>`).join('');
    document.getElementById('res-steps').innerHTML = res.steps.map((s,i)=>`<div class="next-step"><span class="step-num-sm">${i+1}</span><span>${s}</span></div>`).join('');
    document.getElementById('wiz-progress').innerHTML = QUESTIONS[lang].map(()=>`<div class="progress-dot done"></div>`).join('');
  }

  function wizRestart() {
    wizAnswers=[]; wizStep=0;
    document.getElementById('wiz-result').style.display='none';
    document.getElementById('wiz-questions').style.display='block';
    renderQuestion();
  }

  renderQuestion();
