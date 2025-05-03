#include <Arduino.h>
#include "wvfpe.h"
#define log_e(msg) Serial.println(msg)

WvFingerprint::WvFingerprint(HardwareSerial &sr, uint8_t rstPin, uint8_t wakePin) {
    _rstPin = rstPin;
    _wakePin = wakePin;
    _serial = &sr;
};

void WvFingerprint::init() {
    pinMode(_rstPin, OUTPUT);
    pinMode(_wakePin, INPUT);
    //_setSleepMode(true);
    _serial->begin(19200);
    delay(100);
    _timeout = 0;//getTimeout();
}

// deletes all fingerprints
bool WvFingerprint::clearAllUser() {
    byte cmdSend[WVFP_TXRXDATA_SIZE] = {  WVFP_CMD_DEL_ALL, 0x00, 0x00, 0x00 };
    byte cmdRece[WVFP_TXRXDATA_SIZE];
    bool success = false;
    _setSleepMode(false);
    _clearSerialBuffer();
    if (_txAndRxCmd(cmdSend, cmdRece, 100) && cmdRece[0] == WVFP_CMD_DEL_ALL && cmdRece[3] == WVFP_ACK_SUCCESS) {
        success = true;
    } else {
        log_e("cannot delete all users");
    }

    _setSleepMode(true);
    return success;
}

void WvFingerprint::_printBuffer(byte* buffer, int len)
{
  for (int i = 0; i < len; i++) 
  {
    if (buffer[i] < 0x10)
      Serial.print('0');  // Add leading zero
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}

// add a new fingerprint (3 steps required)
bool WvFingerprint::addUser(uint16_t userId, uint8_t step) {
    byte cmdSend[WVFP_TXRXDATA_SIZE] = {};

    Serial.print("timeout = ");
    Serial.print(_timeout);
    Serial.println(" secs");

    switch (step) {
        case 1: cmdSend[0] = WVFP_CMD_ADD_1; break;
        case 2: cmdSend[0] = WVFP_CMD_ADD_2; break;
        case 3: cmdSend[0] = WVFP_CMD_ADD_3; break;
        default: return false;
    }

    // userId
    cmdSend[1] = (userId & 0xff00) >> 8;
    cmdSend[2] = userId & 0x00ff;
    cmdSend[3] = 0x3; // Permission

    byte cmdRece[WVFP_TXRXDATA_SIZE] = {0};
    bool success = false;
    _setSleepMode(false);
    _clearSerialBuffer();
    if (_txAndRxCmd(cmdSend, cmdRece, _timeout == 0 ? 1000 : _timeout*1000) && 
        cmdRece[0] == cmdSend[0] && 
        cmdRece[3] == WVFP_ACK_SUCCESS) 
    {
        success = true;
    } else {
        Serial.print("sent buffer: ");
        _printBuffer(_cmdSendBuffer, WVFP_TXRXBUFFER_SIZE);
        Serial.print("receiver buffer: ");
        _printBuffer(_cmdReceiverBuffer, WVFP_TXRXBUFFER_SIZE);
        _lastError = cmdRece[3];
        log_e("cannot add user");
        log_e(_lastError);
    }

    _setSleepMode(true);
    return success;
}

// returns the Serial Number (default 0)
uint32_t WvFingerprint::getSerialNumber() {
    byte serialNoCmd[WVFP_TXRXDATA_SIZE] = {  WVFP_CMD_SERIALNO };
    byte serialNoRsp[WVFP_TXRXDATA_SIZE];
    uint32_t serialNo = 0;

    _setSleepMode(false);
    _clearSerialBuffer();
    if (_txAndRxCmd(serialNoCmd, serialNoRsp, 100) && serialNoRsp[0] == WVFP_CMD_SERIALNO) {
        serialNo = serialNoRsp[1];
        serialNo = serialNo << 8;
        serialNo = serialNo | serialNoRsp[2];
        serialNo = serialNo << 8;
        serialNo = serialNo  | serialNoRsp[3];
    } else {
        _lastError = serialNoRsp[0];
        log_e("cannot get serialNo");
    }
    _setSleepMode(true);
    return serialNo;
}

// returns the number of user
// Fingerprint capacity: 500
uint16_t WvFingerprint::getUserCount() {
    byte cmdSend[WVFP_TXRXDATA_SIZE] = {  WVFP_CMD_USER_CNT };
    byte cmdRece[WVFP_TXRXDATA_SIZE];
    uint16_t userCnt = 0;
    _setSleepMode(false);
    _clearSerialBuffer();
    if (_txAndRxCmd(cmdSend, cmdRece, 100) && cmdRece[0] == WVFP_CMD_USER_CNT && cmdRece[3] == WVFP_ACK_SUCCESS) {
        userCnt = cmdRece[1];
        userCnt = userCnt << 8;
        userCnt = userCnt | cmdRece[2];

    } else {
        _lastError = cmdRece[3];
        log_e("cannot get user count");
    }

    _setSleepMode(true);
    return userCnt;
}

// Returns the comparison level 0-9
uint8_t WvFingerprint::getCompareLevel() {
    byte cmdSend[WVFP_TXRXDATA_SIZE] = {  WVFP_CMD_COM_LEV, 0x00, 0x00, 0x01 };
    byte cmdRece[WVFP_TXRXDATA_SIZE];
    uint8_t compareLevel = 0;
    _setSleepMode(false);
    _clearSerialBuffer();
    if (_txAndRxCmd(cmdSend, cmdRece, 100) && cmdRece[0] == WVFP_CMD_COM_LEV && cmdRece[3] == WVFP_ACK_SUCCESS) {
        compareLevel = cmdRece[2];
    } else {
        _lastError = cmdRece[3];
        log_e("cannot get compare level");
    }

    _setSleepMode(true);
    return compareLevel;
}

// sets the compare level 0-9
bool WvFingerprint::setCompareLevel(uint8_t newLevel) {
    if (newLevel >= 0 && newLevel <= 9) {
        byte cmdSend[WVFP_TXRXDATA_SIZE] = {  WVFP_CMD_COM_LEV, 0x00, newLevel, 0x00 };
        byte cmdRece[WVFP_TXRXDATA_SIZE];
        uint8_t compareLevel = 0;
        _setSleepMode(false);
        _clearSerialBuffer();
        if (_txAndRxCmd(cmdSend, cmdRece, 100) && cmdRece[0] == WVFP_CMD_COM_LEV && cmdRece[3] == WVFP_ACK_SUCCESS) {
            compareLevel = cmdRece[2];
        } else {
            _lastError = cmdRece[3];
            log_e("cannot set compare level");
        }

        _setSleepMode(true);
        return compareLevel == newLevel;
    } else {
        return false;
    }
}

// gets the timeout (seconds)
uint8_t WvFingerprint::getTimeout() {
    byte cmdSend[WVFP_TXRXDATA_SIZE] = {  WVFP_CMD_TIMEOUT, 0x00, 0x00, 0x01 };
    byte cmdRece[WVFP_TXRXDATA_SIZE];
    uint8_t compareLevel = 0;
    _setSleepMode(false);
    _clearSerialBuffer();
    if (_txAndRxCmd(cmdSend, cmdRece, 100) && cmdRece[0] == WVFP_CMD_TIMEOUT && cmdRece[3] == WVFP_ACK_SUCCESS) {
        _timeout = cmdRece[2];
    } else {
        log_e("cannot get timeout");
    }

    _setSleepMode(true);
    return compareLevel;
}

// sets the timeout (in seconds)
bool WvFingerprint::setTimeout(uint8_t newTimeout) {
    byte cmdSend[WVFP_TXRXDATA_SIZE] = {  WVFP_CMD_TIMEOUT, 0x00, newTimeout, 0x00 };
    byte cmdRece[WVFP_TXRXDATA_SIZE];
    uint8_t timeout = 0;
    _setSleepMode(false);
    _clearSerialBuffer();
    if (_txAndRxCmd(cmdSend, cmdRece, 100) && cmdRece[0] == WVFP_CMD_TIMEOUT && cmdRece[3] == WVFP_ACK_SUCCESS) {
        timeout = cmdRece[2];
    } else {
        _lastError = cmdRece[3];
        log_e("cannot set timeout");
    }

    if (timeout == newTimeout) {
        _timeout = newTimeout;
    }

    _setSleepMode(true);
    return timeout == newTimeout;
}

// check if there is a fingerprint available
// if yes, return the userId (1-500)
// no finger present: return 0
// finger present but error: 0xFFFF
uint16_t WvFingerprint::checkForFingerprint() {
    if (digitalRead(_wakePin) == HIGH) {
        byte cmdSend[WVFP_TXRXDATA_SIZE] = {  WVFP_CMD_MATCH };
        byte cmdRece[WVFP_TXRXDATA_SIZE];
        uint16_t userId;

        _setSleepMode(false);
        _clearSerialBuffer();
        if (_txAndRxCmd(cmdSend, cmdRece, (_timeout*1000)+100) && cmdRece[0] == cmdSend[0]) {
            if (cmdRece[3] == WVFP_ACK_GUEST_USER || cmdRece[3] == WVFP_ACK_MASTER_USER || cmdRece[3] == WVFP_ACK_NORMAL_USER) {
                userId = cmdRece[1];
                userId = userId << 8;
                userId = userId | cmdRece[2];

            } else {
                _lastError = cmdRece[3];
                userId = 0xFFFF;
            }
        } else {
            Serial.print("sent buffer: ");
            _printBuffer(_cmdSendBuffer, WVFP_TXRXBUFFER_SIZE);
            Serial.print("receiver buffer: ");
            _printBuffer(_cmdReceiverBuffer, WVFP_TXRXBUFFER_SIZE);
            _lastError = cmdRece[3];
            userId = 0xFFFF;
            log_e("cannot get fingerprint");
        }

        _setSleepMode(true);
        return userId;
    }
    return 0;
}

// returns the last error code
uint8_t WvFingerprint::getLastError() {
    return _lastError;
}


// ***************************************************************************
// PRIVATE
// ***************************************************************************

void WvFingerprint::_clearTxBuffer() {
    for (uint8_t i=0; i<WVFP_TXRXBUFFER_SIZE; i++) {
        _cmdSendBuffer[i] = 0x00;
    }
}

void WvFingerprint::_clearRxBuffer() {
    for (uint8_t i=0; i<WVFP_TXRXBUFFER_SIZE; i++) {
        _cmdReceiverBuffer[i] = 0x00;
    }
}

void WvFingerprint::_clearSerialBuffer() {
    while(_serial->available()) {
        _serial->read();
    }
}

// true = sleep mode, false = active
void WvFingerprint::_setSleepMode(bool sleep) {
    digitalWrite(_rstPin, sleep ? LOW : HIGH);
    if (!sleep) {
        delay(200); // give time to wake up
    }
}

void WvFingerprint::_txCmd(byte commands[]) {

    memset(_cmdSendBuffer, 0, WVFP_TXRXBUFFER_SIZE);

    _cmdSendBuffer[0] = WVFP_CMD_HEAD;
    for (uint8_t i = 0; i<4; i++) {
        _cmdSendBuffer[1+i] = commands[i];
    }
    _cmdSendBuffer[5] = 0;

    byte checksum = 0;
    for (uint8_t i = 1; i<6; i++) {
        checksum = checksum ^ _cmdSendBuffer[i];
    }
    _cmdSendBuffer[6] = checksum;
    _cmdSendBuffer[7] = WVFP_CMD_TAIL;

    _serial->write(_cmdSendBuffer, WVFP_TXRXBUFFER_SIZE);
    _serial->flush();
}

bool WvFingerprint::_rxCmd(byte response[]) {

    memset(_cmdReceiverBuffer, 0, WVFP_TXRXBUFFER_SIZE);

    size_t numrcvd = _serial->readBytes(_cmdReceiverBuffer, WVFP_TXRXBUFFER_SIZE);
    if (numrcvd != WVFP_TXRXBUFFER_SIZE) {
        Serial.print("error: received only ");
        Serial.print(numrcvd);
        Serial.println(" bytes");
        return false;
    }

    if (_cmdReceiverBuffer[0] == WVFP_CMD_HEAD && _cmdReceiverBuffer[WVFP_TXRXBUFFER_SIZE - 1] == WVFP_CMD_TAIL) {
        byte checksum = 0;
        for (uint8_t i = 1; i<6; i++) {
            checksum ^= _cmdReceiverBuffer[i];
        }

        // check if checksum is correct
        if (checksum == _cmdReceiverBuffer[WVFP_TXRXBUFFER_SIZE - 2]) {
            for (uint8_t i = 0; i < WVFP_TXRXDATA_SIZE; i++) {
                response[i] = _cmdReceiverBuffer[i+1];
            }
            return true;
        } else {
            log_e("invalid checksum");
        }
    }

    return false;
}


/**
 * Send a command and wait for the respnse
 **/
bool WvFingerprint::_txAndRxCmd(byte commands[], byte response[], uint32_t timeout) {
    _txCmd(commands);
    delay(100);
    unsigned long waitUntil = millis() + timeout;
    bool success = false;
    while(!(success = _rxCmd(response)) && millis() < waitUntil) {
        delay(10);
    }

    return success;
}