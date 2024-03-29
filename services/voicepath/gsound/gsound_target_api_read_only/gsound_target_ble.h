// Copyright 2018 Google LLC.
// Libgsound version: f5d8a0a
#ifndef GSOUND_TARGET_BLE_H
#define GSOUND_TARGET_BLE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This header file was generated by Google GSound Services,
 * it should not be altered in any way. Please treat this as
 * a read-only file.
 *
 * Each function listed below must
 * be implemented for each specific
 * platform / SDK
 *
 */
#include "gsound.h"

/**
 * Following declares the standard BLE
 * Interface API.
 */
typedef struct {
  /**
   * Callback which should be executed by Target to Synchronously indicate that
   * a channel has been connected. The addr parameter MUST be the public BD
   * address of the remote device.
   *
   * A GSound BLE Channel is considered connected when notifications have
   * been enabled in the Client Characteristic Configuration Descriptor (CCCD)
   * for the GSound Tx Characteristic of specific specific channel.
   * For a GSound BLE Channel to be considered connected, the underlying
   * BLE GAP link has to be connected.
   *
   * Depending on the return value of this function, Target should either
   * accept the CCCD change and enable notifications, or reject the change
   * and return error 0x80 "Application error" to the GATT Client.
   * See return values below.
   *
   * Multi-Threaded Environment:
   *    - This method is a blocking call that waits on a Mutex. Therefore this
   *      method is not ISR safe.
   *
   * param[in]: interval  Valid values would be from 6 to 3200 (7.5 ms to 4.0s)
   * The value is not checked by libgsound but passed to the phone.
   *
   * param[in]: slave_latency  Valid values would be from 0 to 500. The value is
   * not checked by libgsound but passed to the phone.
   *
   * Returns  GSOUND_STATUS_OK    If the connection should be allowed and
   *                              GATT notifications enabled.
   *          GSOUND_STATUS_ERROR If the connection should be rejected as
   *                              noted above.
   */
  GSoundStatus (*gsound_ble_on_connect)(GSoundChannelType channel,
                                        const GSoundBTAddr *addr,
                                        uint16_t interval,
                                        uint16_t slave_latency);
  /**
   * Callback which should be executed by Target to Synchronously indicate that
   * a channel has been disconnected.
   *
   * A GSound BLE Channel is considered disconnected when notifications have
   * been disabled in the Client Characteristic Configuration Descriptor (CCCD)
   * for the GSound Tx Characteristic of specific channel.
   * A GSound BLE Channel is also considered disconnected if the underlying
   * BLE GAP link has disconnected.
   *
   * Multi-Threaded Environment:
   *    - This method is a blocking call that waits on a Mutex. Therefore this
   *      method is not ISR safe.
   *
   * Returns GSOUND_STATUS_ERROR if the channel is not currently connected and
   * GSOUND_STATUS_OK otherwise.
   */
  GSoundStatus (*gsound_ble_on_disconnect)(GSoundChannelType channel);

  /**
   * Callback which should be executed by Target when a transmission
   * over a BLE channel has completed.
   *
   * Target must provide the `channel`, `data`, and `length` parameters to
   * gsound_ble_on_tx_consumed which exactly match the `channel`, `data`, and
   * `length` parameters passed to GSoundTargetBleTransmit.
   *
   * This function is non-blocking and thread safe. It can be called from any
   * thread/context including an ISR.
   */
  GSoundStatus (*gsound_ble_on_tx_consumed)(GSoundChannelType channel,
                                            const uint8_t *data,
                                            uint32_t length);

  /**
   * Callback which should be executed by Target when there is transmit space
   * available for GATT Notification on the channel. This call is required if a
   * call to GSoundTargetBleTransmit failed to consume or queue all of the data.
   * See GSoundTargetBleTransmit for more information.
   *
   * This function is non-blocking and thread safe. It can be called from any
   * thread/context including an ISR.
   */
  GSoundStatus (*gsound_ble_on_tx_available)(GSoundChannelType channel);

  /**
   * Callback which should be executed by Target when data has been
   * received from remote device.
   *
   * Target must provide a pointer to a buffer containing the data which was
   * received along with its length and the specific channel.
   *
   * GSound will consume this data asynchronously from within the provided
   * buffer. Therefore Target cannot free this buffer after this call
   * returns. Instead, Target must reserve the buffer for GSound to use
   * until GSound calls GSoundTargetBleRxComplete
   *
   * This function is non-blocking and thread safe. It can be called from any
   * thread/context including an ISR.
   *
   */
  GSoundStatus (*gsound_ble_on_rx_ready)(GSoundChannelType channel,
                                         const uint8_t *buffer, uint32_t len);

  /**
   * Callback which should be executed by Target when a LL_CONNECTION_UPDATE_IND
   * LL Control PDU is received.
   *
   * The values passed in for interval and slave_latency should be what is in
   * the LL_CONNECTION_UPDATE_IND PDU without conversion.
   *
   * The initial values are set by the call gsound_ble_on_connect() and updated
   * with this call.  Any values passed to Libgsound are passed up to the phone.
   *
   * By the BLE specification:
   * Valid values for interval would be from 6 to 3200 (7.5 ms to 4.0s.)
   * Valid values for slave_latency would be from 0 to 500.
   *
   * param[in]: interval  The interval value from the PDU.  The value is not
   * checked by libgsound but passed to the phone.
   *
   * param[in]: slave_latency  The slave_latency value from the PDU. The value
   * is not checked by libgsound but passed to the phone.
   *
   * return:
   *    GSOUND_STATUS_OK:            If the values were successfully queued
   *                                 for transmission.
   *
   *    GSOUND_STATUS_ERROR:         If any error occurs.
   */
  GSoundStatus (*gsound_ble_on_param_change)(uint16_t interval,
                                             uint16_t slave_latency);

} GSoundBleInterface;

/**
 * Transmit a BLE GATT Notifification over the provided channel.
 *
 * The target can choose to transmit the data directly from the GSound buffer
 * (without making a copy), or immediately make a copy before returning from
 * this function.
 *
 * If a copy is made, Target must set the `bytes_consumed` parameter to the
 * number of bytes copied before returning.
 *
 * If a copy is not made, Target must call the function
 * GSoundBleInterface.gsound_ble_on_tx_consumed(channel, data, length) at some
 * later time.
 *
 * param[in]:  channel GSound channel to send data on.
 * param[in]:  data    Pointer to an array of `length` bytes
 * param[in]:  length  Number of bytes to send. This value will be no more than
 *                     one MTU (as reported by GSoundTargetBleGetMtu)
 * param[out]: bytes_consumed Target must set this value to the number of bytes
 *                            consumed or copied before transmitting. If the
 *                            data is transmitted without making a copy, set
 *                            this value to 0.
 *
 * return:
 *    GSOUND_STATUS_OK:            If all `length` bytes of data has been
 *                                 successfully queued or consumed for
 *                                 transmission.
 *
 *    GSOUND_STATUS_OUT_OF_MEMORY: If less than `length` bytes could be consumed
 *                                 or queued by Target for transmission. In this
 *                                 case Target must call
 *                                 GSoundBleInterface.gsound_bt_on_tx_available()
 *                                 when memory is available.
 *    GSOUND_STATUS_ERROR:         If any other error occurs.
 */
GSoundStatus GSoundTargetBleTransmit(GSoundChannelType channel,
                                     const uint8_t *data, uint32_t length,
                                     uint32_t *bytes_consumed);

/**
 * Callback from GSound to Target platform. This function will be called
 * by GSound when the buffer passed to GSound from Target via
 * the GSoundBleInterface.gsound_ble_on_rx_ready(...) has been processed
 * and the memory can be freed by target.
 *
 * For example:
 *    - gsound_ble_on_rx_ready(...) --> to GSound will start consuming buffer
 *      ...
 *    - (GSound still consuming buffer))
 *      ...
 *    - (GSound done using buffer)
 *    - GSoundTargetBleRxComplete(...) --> To Target, ready to free buffer
 *    - (Target frees buffer)
 */
void GSoundTargetBleRxComplete(GSoundChannelType type, const uint8_t *buffer,
                               uint32_t len);

/**
 * Return the current negotiated BLE MTU for GATT writes or reads.
 *
 * This function should return the number of bytes available to gsound to send
 * in a single transmission unit. This value must exclude the number of ATT
 * bytes required to transmit a message.
 *
 * GSound will break up any packets larger than the MTU size returned by this
 * function before transmitting.
 *
 * Note: GSound will NOT buffer smaller packets to make up a full MTU before
 * transmitting. Instead, GSound will always transmit whenever it has any data
 * to send. It is up to the Target to buffer small data packets into a single
 * MTU before transmitting if this optimization is required.
 *
 * Note that GSound expects MTU to always be non-zero.  If, however, the target
 * does return an MTU of zero at some point and later returns the MTU to a
 * non-zero value without a disconnect, it must send
 * gsound_ble_on_tx_available() when MTU is changed from zero to non-zero. This
 * prevents transmit stalls within GSound.
 */
uint16_t GSoundTargetBleGetMtu(void);

/**
 * Transmit a LL_CONNECTION_PARAM_REQ PDU.  The other values in the PDU not
 * specified by these arguements should use the current value.
 * If valid_interval is true, use the min_interval and max_interval values in
 * the PDU, otherwise use the current value.
 * If valid_slave_latency is true, use slave_latency value in the PDU,
 * otherwise use the current value.
 *
 * The values passed should be put in the LL_CONNECTION_PARAM_REQ PDU without
 * conversion.  They are in units the PDU expects.
 *
 * It is expected that this will cause a LL_CONNECTION_UPDATE_IND or
 * LL_REJECT_IND_EXT PDU to be received. If a LL_CONNECTION_UPDATE_IND is
 * received a call to gsound_ble_on_param_change() should be made. If a
 * LL_REJECT_IND_EXT is received then no call is required to
 * gsound_ble_on_param_change() as no change has occured.
 */
GSoundStatus GSoundTargetBleUpdateConnParams(bool valid_interval,
                                             uint32_t min_interval,
                                             uint32_t max_interval,
                                             bool valid_slave_latency,
                                             uint32_t slave_latency);
/**
 * Initialize and create all GATT services required
 * by GSound. This includes GSound, BMS, AMSP, and ANCSP.
 * Refer to BLE documented provided by Google for more information
 *
 * Target platform needs to add the following 2 items into BLE advertisement:
 *    1) Service (type 0x16) with GSOUND UUID ( FE26 ) and Device ID in
 *       data field.
 *       - Device ID is assigned by Google when SKU registered with the
 *       Google database.
 *    2) Complete Local Name (type 0x09)
 *       - The Advertised name should be complete name with �LE-� prepended or
 *       "-LE" appended.
 *
 * NOTE: for TWS, only start advertising on master.
 * NOTE: GSound requires a 15ms connection interval to operate properly.  Target
 *       must request a BLE connection interval of 15ms for both min and max
 *       values.
 *
 * Please also consider the following guidelines from Apple. Section 10.6 of
 * https://developer.apple.com/accessories/Accessory-Design-Guidelines.pdf
 *
 *
 */
GSoundStatus GSoundTargetBleInit(const GSoundBleInterface *handlers);

#ifdef __cplusplus
}
#endif

#endif  // GSOUND_TARGET_BLE_H
