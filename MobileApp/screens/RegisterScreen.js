// screens/RegisterScreen.js
import React, { useState } from 'react';
import {
  View,
  Text,
  TextInput,
  TouchableOpacity,
  StyleSheet,
  ImageBackground,
  KeyboardAvoidingView,
  Platform,
} from 'react-native';
import { Ionicons } from '@expo/vector-icons';

export default function RegisterScreen({ navigation }) {
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [confirmPw, setConfirmPw] = useState('');

  const handleRegister = async () => {
    if (!email || !password || !confirmPw) {
      alert('모든 항목을 입력하세요.');
      return;
    }

    if (password !== confirmPw) {
      alert('비밀번호가 일치하지 않습니다.');
      return;
    }

    try {
      const res = await fetch('http://210.119.12.64:5000/register', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ email, password }),
      });

      const data = await res.json();

      if (res.ok) {
        alert(data.message); // "회원가입 성공!"
        navigation.replace('Login'); // 로그인 화면으로 이동
      } else {
        alert(data.message || '회원가입 실패');
      }
    } catch (err) {
      console.error(err);
      alert('서버에 연결할 수 없습니다.');
    }
  }

  return (
    <ImageBackground
      source={{ uri: 'https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRjFL3-jwGmb8O6GJ0kgolrJuoGXahDTyVt-Q&s' }}
      style={styles.background}
      blurRadius={3}
    >
      <KeyboardAvoidingView
        behavior={Platform.OS === 'ios' ? 'padding' : undefined}
        style={styles.container}
      >
        <Text style={styles.title}>회원가입</Text>

        <View style={styles.inputContainer}>
          <Ionicons name="mail-outline" size={20} color="#333" style={styles.icon} />
          <TextInput
            placeholder="이메일"
            value={email}
            onChangeText={setEmail}
            style={styles.input}
            keyboardType="email-address"
          />
        </View>

        <View style={styles.inputContainer}>
          <Ionicons name="lock-closed-outline" size={20} color="#333" style={styles.icon} />
          <TextInput
            placeholder="비밀번호"
            value={password}
            onChangeText={setPassword}
            secureTextEntry
            style={styles.input}
          />
        </View>

        <View style={styles.inputContainer}>
          <Ionicons name="lock-closed-outline" size={20} color="#333" style={styles.icon} />
          <TextInput
            placeholder="비밀번호 확인"
            value={confirmPw}
            onChangeText={setConfirmPw}
            secureTextEntry
            style={styles.input}
          />
        </View>

        <TouchableOpacity style={styles.registerButton} onPress={handleRegister}>
          <Text style={styles.registerText}>회원가입</Text>
        </TouchableOpacity>

        <TouchableOpacity onPress={() => navigation.navigate('Login')}>
          <Text style={styles.link}>이미 계정이 있으신가요? 로그인</Text>
        </TouchableOpacity>
      </KeyboardAvoidingView>
    </ImageBackground>
  );
}

const styles = StyleSheet.create({
  background: {
    flex: 1,
    resizeMode: 'cover',
    justifyContent: 'center',
  },
  container: {
    padding: 25,
    backgroundColor: 'rgba(255,255,255,0.85)',
    margin: 20,
    borderRadius: 15,
  },
  title: {
    fontSize: 28,
    fontWeight: '700',
    textAlign: 'center',
    marginBottom: 30,
    color: '#333',
  },
  inputContainer: {
    flexDirection: 'row',
    alignItems: 'center',
    borderColor: '#aaa',
    borderWidth: 1,
    borderRadius: 8,
    marginBottom: 15,
    paddingHorizontal: 10,
    backgroundColor: '#fff',
  },
  icon: {
    marginRight: 8,
  },
  input: {
    flex: 1,
    height: 45,
    fontSize: 16,
  },
  registerButton: {
    backgroundColor: '#4a7c59',
    paddingVertical: 12,
    borderRadius: 8,
    marginTop: 10,
  },
  registerText: {
    color: '#fff',
    textAlign: 'center',
    fontSize: 17,
    fontWeight: '600',
  },
  link: {
    marginTop: 20,
    textAlign: 'center',
    color: '#333',
    textDecorationLine: 'underline',
  },
});
