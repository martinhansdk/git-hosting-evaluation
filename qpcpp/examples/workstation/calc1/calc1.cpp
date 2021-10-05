//.$file${.::calc1.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: calc1.qm
// File:  ${.::calc1.cpp}
//
// This code has been generated by QM 5.1.0 <www.state-machine.com/qm/>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
//.$endhead${.::calc1.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpcpp.hpp" // QP/C++
#include "bsp.hpp"   // board support package
#include "calc1.hpp" // application

Q_DEFINE_THIS_FILE

#define KEY_NULL    '\0'
#define KEY_PLUS    '+'
#define KEY_MINUS   '-'
#define KEY_MULT    '*'
#define KEY_DIVIDE  '/'

//.$declare${SMs::Calc} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//.${SMs::Calc} ..............................................................
class Calc : public QP::QHsm {
public:
    static Calc inst;

private:
    double m_op1;
    double m_op2;
    uint8_t m_oper1;
    uint8_t m_oper2;

public:
    Calc();

    // guard function to evaluate the current expression
    // taking into account the precedence of operands.
    // return: true if evaluation successfull
    // false when error encountered
    bool eval(double op, uint8_t oper);

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(on);
    Q_STATE_DECL(ready);
    Q_STATE_DECL(begin);
    Q_STATE_DECL(result);
    Q_STATE_DECL(operand1);
    Q_STATE_DECL(zero1);
    Q_STATE_DECL(int1);
    Q_STATE_DECL(frac1);
    Q_STATE_DECL(negated1);
    Q_STATE_DECL(opEntered);
    Q_STATE_DECL(operand2);
    Q_STATE_DECL(zero2);
    Q_STATE_DECL(int2);
    Q_STATE_DECL(frac2);
    Q_STATE_DECL(negated2);
    Q_STATE_DECL(error);
    Q_STATE_DECL(final);
};
//.$enddecl${SMs::Calc} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//.$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//. Check for the minimum required QP version
#if (QP_VERSION < 680U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 6.8.0 or higher required
#endif
//.$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//.$define${SMs::the_calc} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//.${SMs::the_calc} ..........................................................
QP::QHsm * const the_calc = &Calc::inst;
//.$enddef${SMs::the_calc} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//.$define${SMs::Calc} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//.${SMs::Calc} ..............................................................
Calc Calc::inst;
//.${SMs::Calc::Calc} ........................................................
Calc::Calc()
 : QHsm(Q_STATE_CAST(&Calc::initial))
{}


// guard function to evaluate the current expression
// taking into account the precedence of operands.
// return: true if evaluation successfull
// false when error encountered
//.${SMs::Calc::eval} ........................................................
bool Calc::eval(double op, uint8_t oper) {
    double result;
    if ((oper == KEY_NULL) || (oper == KEY_PLUS) || (oper == KEY_MINUS)) {
        switch (m_oper2) {
            case KEY_MULT: {
                m_op2 *= op;
                break;
            }
            case KEY_DIVIDE: {
                if ((-1e-30 < op) && (op < 1e-30)) {
                    BSP_display_error(" Error 0 "); // divide by zero
                    return false;
                }
                m_op2 /= op;
                break;
             }
             default: { /* no op2 yet */
                m_op2 = op;
                m_oper2 = oper;
                break;
             }
        }
        switch (m_oper1) {
            case KEY_PLUS: {
                m_op1 += m_op2;
                break;
            }
            case KEY_MINUS: {
                m_op1 -= m_op2;
                break;
            }
            case KEY_MULT: {
                m_op1 *= m_op2;
                break;
            }
            case KEY_DIVIDE: {
                if ((-1e-30 < m_op2) && (m_op2 < 1e-30)) {
                    BSP_display_error(" Error 0 "); // divide by zero
                    return false;
                }
                m_op1 /= m_op2;
                break;
            }
            default: {
                Q_ERROR();
                break;
            }
        }
        m_oper1 = oper;
        m_oper2 = KEY_NULL;
        result = m_op1;
    }
    else { // (oper == KEY_MULT) || (oper == KEY_DIV)
        switch (m_oper2) {
            case KEY_MULT: {
                m_op2 *= op;
                break;
            }
            case KEY_DIVIDE: {
                if ((-1e-30 < op) && (op < 1e-30)) {
                    BSP_display_error(" Error 0 "); // divide by zero
                    return false;
                }
                m_op2 /= op;
                break;
            }
            default: { // oper2 not provided yet
                m_op2 = op;
                break;
            }
        }
        m_oper2 = oper;
        result = m_op2;
    }

    if ((result < -99999999.0) || (99999999.0 < result)) {
        BSP_display_error(" Error 1 "); // out of range
        return false;
    }
    if ((-0.0000001 < result) && (result < 0.0000001)) {
        result = 0.0;
    }
    BSP_display(result);

    return true;
}

//.${SMs::Calc::SM} ..........................................................
Q_STATE_DEF(Calc, initial) {
    //.${SMs::Calc::SM::initial}
    BSP_clear();
    (void)e; /* unused parameter */

    QS_FUN_DICTIONARY(&Calc::on);
    QS_FUN_DICTIONARY(&Calc::ready);
    QS_FUN_DICTIONARY(&Calc::begin);
    QS_FUN_DICTIONARY(&Calc::result);
    QS_FUN_DICTIONARY(&Calc::operand1);
    QS_FUN_DICTIONARY(&Calc::zero1);
    QS_FUN_DICTIONARY(&Calc::int1);
    QS_FUN_DICTIONARY(&Calc::frac1);
    QS_FUN_DICTIONARY(&Calc::negated1);
    QS_FUN_DICTIONARY(&Calc::opEntered);
    QS_FUN_DICTIONARY(&Calc::operand2);
    QS_FUN_DICTIONARY(&Calc::zero2);
    QS_FUN_DICTIONARY(&Calc::int2);
    QS_FUN_DICTIONARY(&Calc::frac2);
    QS_FUN_DICTIONARY(&Calc::negated2);
    QS_FUN_DICTIONARY(&Calc::error);
    QS_FUN_DICTIONARY(&Calc::final);

    return tran(&on);
}
//.${SMs::Calc::SM::on} ......................................................
Q_STATE_DEF(Calc, on) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on}
        case Q_ENTRY_SIG: {
            BSP_message("on-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on}
        case Q_EXIT_SIG: {
            BSP_message("on-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::initial}
        case Q_INIT_SIG: {
            BSP_message("on-INIT;");
            status_ = tran(&ready);
            break;
        }
        //.${SMs::Calc::SM::on::C}
        case C_SIG: {
            BSP_clear();
            status_ = tran(&on);
            break;
        }
        //.${SMs::Calc::SM::on::OFF}
        case OFF_SIG: {
            status_ = tran(&final);
            break;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::ready} ...............................................
Q_STATE_DEF(Calc, ready) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::ready}
        case Q_ENTRY_SIG: {
            BSP_message("ready-ENTRY;");
            m_oper2 = KEY_NULL;
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::ready}
        case Q_EXIT_SIG: {
            BSP_message("ready-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::ready::initial}
        case Q_INIT_SIG: {
            BSP_message("ready-INIT;");
            status_ = tran(&begin);
            break;
        }
        //.${SMs::Calc::SM::on::ready::DIGIT_0}
        case DIGIT_0_SIG: {
            BSP_clear();
            status_ = tran(&zero1);
            break;
        }
        //.${SMs::Calc::SM::on::ready::DIGIT_1_9}
        case DIGIT_1_9_SIG: {
            BSP_clear();
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = tran(&int1);
            break;
        }
        //.${SMs::Calc::SM::on::ready::POINT}
        case POINT_SIG: {
            BSP_clear();
            BSP_insert((int)'0');
            BSP_insert((int)'.');
            status_ = tran(&frac1);
            break;
        }
        //.${SMs::Calc::SM::on::ready::OPER}
        case OPER_SIG: {
            m_op1  = BSP_get_value();
            m_oper1 = Q_EVT_CAST(CalcEvt)->key_code;
            status_ = tran(&opEntered);
            break;
        }
        default: {
            status_ = super(&on);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::ready::begin} ........................................
Q_STATE_DEF(Calc, begin) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::ready::begin}
        case Q_ENTRY_SIG: {
            BSP_message("begin-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::ready::begin}
        case Q_EXIT_SIG: {
            BSP_message("begin-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::ready::begin::OPER}
        case OPER_SIG: {
            //.${SMs::Calc::SM::on::ready::begin::OPER::[e->key=='-']}
            if (Q_EVT_CAST(CalcEvt)->key_code == KEY_MINUS) {
                status_ = tran(&negated1);
            }
            //.${SMs::Calc::SM::on::ready::begin::OPER::[else]}
            else {
                status_ = Q_RET_HANDLED;
            }
            break;
        }
        default: {
            status_ = super(&ready);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::ready::result} .......................................
Q_STATE_DEF(Calc, result) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::ready::result}
        case Q_ENTRY_SIG: {
            BSP_message("result-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::ready::result}
        case Q_EXIT_SIG: {
            BSP_message("result-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        default: {
            status_ = super(&ready);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::operand1} ............................................
Q_STATE_DEF(Calc, operand1) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::operand1}
        case Q_ENTRY_SIG: {
            BSP_message("operand1-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1}
        case Q_EXIT_SIG: {
            BSP_message("operand1-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::CE}
        case CE_SIG: {
            BSP_clear();
            status_ = tran(&begin);
            break;
        }
        //.${SMs::Calc::SM::on::operand1::OPER}
        case OPER_SIG: {
            m_op1  = BSP_get_value();
            m_oper1 = Q_EVT_CAST(CalcEvt)->key_code;
            status_ = tran(&opEntered);
            break;
        }
        //.${SMs::Calc::SM::on::operand1::EQUALS}
        case EQUALS_SIG: {
            status_ = tran(&result);
            break;
        }
        default: {
            status_ = super(&on);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::operand1::zero1} .....................................
Q_STATE_DEF(Calc, zero1) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::operand1::zero1}
        case Q_ENTRY_SIG: {
            BSP_message("zero1-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::zero1}
        case Q_EXIT_SIG: {
            BSP_message("zero1-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::zero1::DIGIT_0}
        case DIGIT_0_SIG: {
            ;
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::zero1::DIGIT_1_9}
        case DIGIT_1_9_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = tran(&int1);
            break;
        }
        //.${SMs::Calc::SM::on::operand1::zero1::POINT}
        case POINT_SIG: {
            BSP_insert((int)'0');
            BSP_insert((int)'.');
            status_ = tran(&frac1);
            break;
        }
        default: {
            status_ = super(&operand1);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::operand1::int1} ......................................
Q_STATE_DEF(Calc, int1) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::operand1::int1}
        case Q_ENTRY_SIG: {
            BSP_message("int1-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::int1}
        case Q_EXIT_SIG: {
            BSP_message("int1-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::int1::POINT}
        case POINT_SIG: {
            BSP_insert((int)'.');
            status_ = tran(&frac1);
            break;
        }
        //.${SMs::Calc::SM::on::operand1::int1::DIGIT_0, DIGIT_1_9}
        case DIGIT_0_SIG: // intentionally fall through
        case DIGIT_1_9_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = Q_RET_HANDLED;
            break;
        }
        default: {
            status_ = super(&operand1);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::operand1::frac1} .....................................
Q_STATE_DEF(Calc, frac1) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::operand1::frac1}
        case Q_ENTRY_SIG: {
            BSP_message("frac1-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::frac1}
        case Q_EXIT_SIG: {
            BSP_message("frac1-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::frac1::POINT}
        case POINT_SIG: {
            ;
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::frac1::DIGIT_0, DIGIT_1_9}
        case DIGIT_0_SIG: // intentionally fall through
        case DIGIT_1_9_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = Q_RET_HANDLED;
            break;
        }
        default: {
            status_ = super(&operand1);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::operand1::negated1} ..................................
Q_STATE_DEF(Calc, negated1) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::operand1::negated1}
        case Q_ENTRY_SIG: {
            BSP_message("negated1-ENTRY;");
            BSP_negate();
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::negated1}
        case Q_EXIT_SIG: {
            BSP_message("negated1-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand1::negated1::DIGIT_0}
        case DIGIT_0_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = tran(&zero1);
            break;
        }
        //.${SMs::Calc::SM::on::operand1::negated1::DIGIT_1_9}
        case DIGIT_1_9_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = tran(&int1);
            break;
        }
        //.${SMs::Calc::SM::on::operand1::negated1::POINT}
        case POINT_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = tran(&frac1);
            break;
        }
        //.${SMs::Calc::SM::on::operand1::negated1::OPER}
        case OPER_SIG: {
            //.${SMs::Calc::SM::on::operand1::negated1::OPER::[e->key=='-']}
            if (Q_EVT_CAST(CalcEvt)->key_code == KEY_MINUS) {
                ;
                status_ = Q_RET_HANDLED;
            }
            //.${SMs::Calc::SM::on::operand1::negated1::OPER::[else]}
            else {
                status_ = Q_RET_HANDLED;
            }
            break;
        }
        default: {
            status_ = super(&operand1);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::opEntered} ...........................................
Q_STATE_DEF(Calc, opEntered) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::opEntered}
        case Q_ENTRY_SIG: {
            BSP_message("opEntered-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::opEntered}
        case Q_EXIT_SIG: {
            BSP_message("opEntered-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::opEntered::DIGIT_0}
        case DIGIT_0_SIG: {
            BSP_clear();
            status_ = tran(&zero2);
            break;
        }
        //.${SMs::Calc::SM::on::opEntered::DIGIT_1_9}
        case DIGIT_1_9_SIG: {
            BSP_clear();
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = tran(&int2);
            break;
        }
        //.${SMs::Calc::SM::on::opEntered::POINT}
        case POINT_SIG: {
            BSP_clear();
            BSP_insert((int)'0');
            BSP_insert((int)'.');
            status_ = tran(&frac2);
            break;
        }
        //.${SMs::Calc::SM::on::opEntered::OPER}
        case OPER_SIG: {
            //.${SMs::Calc::SM::on::opEntered::OPER::[e->key=='-']}
            if (Q_EVT_CAST(CalcEvt)->key_code == KEY_MINUS) {
                status_ = tran(&negated2);
            }
            //.${SMs::Calc::SM::on::opEntered::OPER::[else]}
            else {
                status_ = Q_RET_HANDLED;
            }
            break;
        }
        default: {
            status_ = super(&on);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::operand2} ............................................
Q_STATE_DEF(Calc, operand2) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::operand2}
        case Q_ENTRY_SIG: {
            BSP_message("operand2-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2}
        case Q_EXIT_SIG: {
            BSP_message("operand2-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::CE}
        case CE_SIG: {
            BSP_clear();
            status_ = tran(&opEntered);
            break;
        }
        //.${SMs::Calc::SM::on::operand2::EQUALS}
        case EQUALS_SIG: {
            //.${SMs::Calc::SM::on::operand2::EQUALS::[eval()]}
            if (eval(BSP_get_value(), KEY_NULL)) {
                status_ = tran(&result);
            }
            //.${SMs::Calc::SM::on::operand2::EQUALS::[else]}
            else {
                status_ = tran(&error);
            }
            break;
        }
        //.${SMs::Calc::SM::on::operand2::OPER}
        case OPER_SIG: {
            //.${SMs::Calc::SM::on::operand2::OPER::[eval()]}
            if (eval(BSP_get_value(), Q_EVT_CAST(CalcEvt)->key_code)) {
                status_ = tran(&opEntered);
            }
            //.${SMs::Calc::SM::on::operand2::OPER::[else]}
            else {
                status_ = tran(&error);
            }
            break;
        }
        default: {
            status_ = super(&on);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::operand2::zero2} .....................................
Q_STATE_DEF(Calc, zero2) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::operand2::zero2}
        case Q_ENTRY_SIG: {
            BSP_message("zero2-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::zero2}
        case Q_EXIT_SIG: {
            BSP_message("zero2-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::zero2::DIGIT_0}
        case DIGIT_0_SIG: {
            ;
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::zero2::DIGIT_1_9}
        case DIGIT_1_9_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = tran(&int2);
            break;
        }
        //.${SMs::Calc::SM::on::operand2::zero2::POINT}
        case POINT_SIG: {
            BSP_insert((int)'0');
            BSP_insert((int)'.');
            status_ = tran(&frac2);
            break;
        }
        default: {
            status_ = super(&operand2);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::operand2::int2} ......................................
Q_STATE_DEF(Calc, int2) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::operand2::int2}
        case Q_ENTRY_SIG: {
            BSP_message("int2-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::int2}
        case Q_EXIT_SIG: {
            BSP_message("int2-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::int2::POINT}
        case POINT_SIG: {
            BSP_insert((int)'.');
            status_ = tran(&frac2);
            break;
        }
        //.${SMs::Calc::SM::on::operand2::int2::DIGIT_0, DIGIT_1_9}
        case DIGIT_0_SIG: // intentionally fall through
        case DIGIT_1_9_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = Q_RET_HANDLED;
            break;
        }
        default: {
            status_ = super(&operand2);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::operand2::frac2} .....................................
Q_STATE_DEF(Calc, frac2) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::operand2::frac2}
        case Q_ENTRY_SIG: {
            BSP_message("frac2-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::frac2}
        case Q_EXIT_SIG: {
            BSP_message("frac2-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::frac2::POINT}
        case POINT_SIG: {
            ;
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::frac2::DIGIT_0, DIGIT_1_9}
        case DIGIT_0_SIG: // intentionally fall through
        case DIGIT_1_9_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = Q_RET_HANDLED;
            break;
        }
        default: {
            status_ = super(&operand2);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::operand2::negated2} ..................................
Q_STATE_DEF(Calc, negated2) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::operand2::negated2}
        case Q_ENTRY_SIG: {
            BSP_message("negated2-ENTRY;");
            BSP_negate();
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::negated2}
        case Q_EXIT_SIG: {
            BSP_message("negated2-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::operand2::negated2::DIGIT_0}
        case DIGIT_0_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = tran(&zero2);
            break;
        }
        //.${SMs::Calc::SM::on::operand2::negated2::DIGIT_1_9}
        case DIGIT_1_9_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = tran(&int2);
            break;
        }
        //.${SMs::Calc::SM::on::operand2::negated2::POINT}
        case POINT_SIG: {
            BSP_insert(Q_EVT_CAST(CalcEvt)->key_code);
            status_ = tran(&frac2);
            break;
        }
        //.${SMs::Calc::SM::on::operand2::negated2::OPER}
        case OPER_SIG: {
            //.${SMs::Calc::SM::on::operand2::negated2::OPER::[e->key=='-']}
            if (Q_EVT_CAST(CalcEvt)->key_code == KEY_MINUS) {
                ;
                status_ = Q_RET_HANDLED;
            }
            //.${SMs::Calc::SM::on::operand2::negated2::OPER::[else]}
            else {
                status_ = Q_RET_HANDLED;
            }
            break;
        }
        default: {
            status_ = super(&operand2);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::on::error} ...............................................
Q_STATE_DEF(Calc, error) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::on::error}
        case Q_ENTRY_SIG: {
            BSP_message("error-ENTRY;");
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${SMs::Calc::SM::on::error}
        case Q_EXIT_SIG: {
            BSP_message("error-EXIT;");
            status_ = Q_RET_HANDLED;
            break;
        }
        default: {
            status_ = super(&on);
            break;
        }
    }
    return status_;
}
//.${SMs::Calc::SM::final} ...................................................
Q_STATE_DEF(Calc, final) {
    QP::QState status_;
    switch (e->sig) {
        //.${SMs::Calc::SM::final}
        case Q_ENTRY_SIG: {
            BSP_message("final-ENTRY;");
            BSP_exit();
            status_ = Q_RET_HANDLED;
            break;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
//.$enddef${SMs::Calc} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
