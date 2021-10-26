import 'dart:convert';
import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

import 'dart:developer';

class SendSignalPage extends StatefulWidget {
  final BluetoothDevice server;

  const SendSignalPage({required this.server});

  @override
  _SendSignalPage createState() => new _SendSignalPage();
}

class _SendSignalPage extends State<SendSignalPage> {
  static final int offState = 0;
  static final int onState = 1;
  static final String offStateMsg = "0";
  static final String onStateMsg = "1";

  BluetoothConnection? connection;
  int _state = offState;
  bool isConnecting = true;
  bool get isConnected => (connection?.isConnected ?? false);
  bool isDisconnecting = false;
  Color _color = Colors.white;

  @override
  void initState() {
    super.initState();
    BluetoothConnection.toAddress(widget.server.address).then((_connection) {
      print('Connected to the device');
      connection = _connection;
      setState(() {
        isConnecting = false;
        isDisconnecting = false;
      });
      connection!.input!.listen(_onDataReceived).onDone(() {
        if (isDisconnecting) {
          print('Disconnecting locally!');
        } else {
          print('Disconnected remotely!');
        }
        if (this.mounted) {
          setState(() {});
        }
      });
    }).catchError((error) {
      print('Cannot connect, exception occured');
      print(error);
    });
  }

  @override
  void dispose() {
    // Avoid memory leak (`setState` after dispose) and disconnect
    if (isConnected) {
      isDisconnecting = true;
      connection?.dispose();
      connection = null;
    }
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final serverName = widget.server.name ?? "Unknown";
    return Scaffold(
      backgroundColor: _color,
      appBar: AppBar(
          title: (isConnecting
              ? Text('Connecting device to ' + serverName + '...')
              : isConnected
                  ? Text('Send signal to ' + serverName)
                  : Text('Not connected to ' + serverName))),
      body: Center(),
      floatingActionButton: FloatingActionButton(
        onPressed: _sendMessage,
        tooltip: 'Power On',
        child: Icon(Icons.power_settings_new),
      ),
    );
  }

  void _onDataReceived(Uint8List data) {
    String dataReceivedString = String.fromCharCodes(data);
    log("_onDataReceived: " + dataReceivedString);
    setState(() {
      _state = int.parse(dataReceivedString);
      (_state == onState) ? _color = Colors.red : _color = Colors.white;
    });
  }

  void _sendMessage() async {
    try {
      String message = (_state == onState) ? offStateMsg : onStateMsg;
      connection!.output.add(Uint8List.fromList(utf8.encode(message)));
      await connection!.output.allSent;
    } catch (e) {
      setState(() {});
    }
  }
}
