/*
 * CÓDIGO FINAL DO PROJETO DE ALARME DE PERÍMETRO
 * Versão Completa com TODOS os componentes validados.
 *
 * SENSORES (3):
 * 1. LDR (luz) [Pino A1]: Arma/Desarma o sistema.
 * 2. Potenciômetro [Pino A0]: Ajusta a sensibilidade (distância limite).
 * 3. HC-SR04 (distância) [Pinos 9, 8]: Detecta o intruso.
 *
 * ATUADORES (2):
 * 1. LED Vermelho [Pino 12]: Alarme visual.
 * 2. Buzzer [Pino 11]: Alarme sonoro.
 */

// --- 1. PINOS DOS COMPONENTES ---
const int PINO_POT = A0;      // Potenciômetro
const int PINO_LDR = A1;      // LDR
const int PINO_TRIG = 9;      // PINO DE TRIGGER (ENVIO) do HC-SR04
const int PINO_ECHO = 8;      // PINO DE ECHO (RECEBIMENTO) do HC-SR04
const int PINO_BUZZER = 11;   // Buzzer
const int PINO_LED = 12;      // LED Vermelho

// --- 2. CONSTANTE DE CALIBRAÇÃO ---
// IMPORTANTE: CALIBRE ESTE VALOR!
const int LIMIAR_ESCURO = 400; // Valor de exemplo, ajuste para o seu quarto!

void setup() {
  Serial.begin(9600); // Inicia o Monitor Serial

  // Configura pinos dos atuadores como SAÍDA
  pinMode(PINO_BUZZER, OUTPUT);
  pinMode(PINO_LED, OUTPUT);
  
  // Configura os pinos do sensor HC-SR04
  pinMode(PINO_TRIG, OUTPUT); 
  pinMode(PINO_ECHO, INPUT);  
}

void loop() {
  
  // --- 3. LER TODOS OS 3 SENSORES ---
  int valorLuz = analogRead(PINO_LDR);
  int valorPot = analogRead(PINO_POT);
  long distanciaAtual_cm = lerDistanciaHCSR04();

  // --- 4. PROCESSAR OS DADOS ---
  // Mapeia o potenciômetro (0-1023) para um limite de distância (5cm a 200cm)
  long distanciaLimite_cm = map(valorPot, 0, 1023, 5, 200);

  // --- 5. EXIBIR DADOS NO MONITOR ---
  // (Abra a "lupa" 🔍 na IDE para ver isso!)
  Serial.print("Luz: ");
  Serial.print(valorLuz);
  Serial.print(" | Limite (Pote): ");
  Serial.print(distanciaLimite_cm);
  Serial.print(" cm | Atual (Sensor): ");
  Serial.print(distanciaAtual_cm);
  Serial.println(" cm");

  // --- 6. LÓGICA PRINCIPAL DO ALARME ---
  
  // 1ª CONDIÇÃO: O alarme só arma se estiver escuro
  if (valorLuz < LIMIAR_ESCURO) {
    // Se estiver ESCURO, o sistema está "armado".
    
    // 2ª CONDIÇÃO: O alarme só dispara se o intruso estiver dentro do limite
    if (distanciaAtual_cm < distanciaLimite_cm && distanciaAtual_cm > 0) {
      
      // ALARME DISPARADO!
      Serial.println(">>> ALARME ATIVADO <<<");
      digitalWrite(PINO_LED, HIGH); // Liga LED
      tone(PINO_BUZZER, 3000);      // Liga Buzzer
      
    } else {
      // Se estiver escuro, mas sem intruso (distância atual é MAIOR que o limite).
      // ALARME ARMADO, MAS QUIETO
      digitalWrite(PINO_LED, LOW);  // Desliga LED
      noTone(PINO_BUZZER);          // Desliga Buzzer
    }
    
  } else {
    // Se estiver CLARO, sistema "desarmado".
    // ALARME DESLIGADO (não importa a distância)
    digitalWrite(PINO_LED, LOW);
    noTone(PINO_BUZZER);
  }
  
  delay(200); // Pausa para estabilizar a leitura
}

/*
 * --- 7. FUNÇÃO DO SENSOR DE DISTÂNCIA ---
 * (Necessária para o HC-SR04 de 4 pinos)
 */
long lerDistanciaHCSR04() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);
  long duration = pulseIn(PINO_ECHO, HIGH);
  long cm = duration / 58;
  return cm;
}