import numpy as np
import pytest

from flxscalers import MinMaxScaler


def test_transform_before_fit_raises():
    scaler = MinMaxScaler((0.0, 1.0))
    with pytest.raises(RuntimeError):
        scaler.transform(np.array([[1.0], [2.0]]))


def test_inverse_transform_before_fit_raises():
    scaler = MinMaxScaler((0.0, 1.0))
    with pytest.raises(RuntimeError):
        scaler.inverse_transform(np.array([[1.0], [2.0]]))


def test_fit_transform_maps_column_onto_unit_range():
    X = np.array([[0.0], [5.0], [10.0]])

    out = MinMaxScaler((0.0, 1.0)).fit_transform(X)

    assert out.shape == (3, 1)
    np.testing.assert_allclose(out.ravel(), [0.0, 0.5, 1.0])


def test_each_column_is_scaled_independently():
    X = np.array([[1.0, 10.0], [3.0, 30.0]])

    out = MinMaxScaler((0.0, 1.0)).fit_transform(X)

    np.testing.assert_allclose(out, [[0.0, 0.0], [1.0, 1.0]])


def test_custom_feature_range_is_honoured():
    X = np.array([[0.0], [5.0], [10.0]])

    out = MinMaxScaler((-1.0, 1.0)).fit_transform(X)

    np.testing.assert_allclose(out.ravel(), [-1.0, 0.0, 1.0])


def test_inverse_transform_undoes_transform():
    X = np.array([[2.0, 100.0], [4.0, 200.0], [6.0, 300.0]])

    scaler = MinMaxScaler((0.0, 1.0))
    roundtrip = scaler.inverse_transform(scaler.fit_transform(X))

    np.testing.assert_allclose(roundtrip, X, atol=1e-9)


def test_transform_applies_statistics_captured_at_fit_time():
    scaler = MinMaxScaler((0.0, 1.0))
    scaler.fit(np.array([[0.0], [10.0]]))

    out = scaler.transform(np.array([[5.0], [-5.0], [20.0]]))

    np.testing.assert_allclose(out.ravel(), [0.5, -0.5, 2.0])


def test_fit_transform_preserves_input_shape():
    X = np.arange(1.0, 13.0).reshape(4, 3)

    out = MinMaxScaler((0.0, 1.0)).fit_transform(X)

    assert out.shape == (4, 3)


def test_constant_column_yields_lower_bound_of_feature_range():
    X = np.array([[7.0], [7.0], [7.0]])

    out = MinMaxScaler((0.0, 1.0)).fit_transform(X)

    np.testing.assert_array_equal(out.ravel(), [0.0, 0.0, 0.0])


def test_default_feature_range_is_unit_interval():
    X = np.array([[0.0, 10.0], [5.0, 20.0], [10.0, 30.0]])

    np.testing.assert_allclose(
        MinMaxScaler().fit_transform(X),
        [[0.0, 0.0], [0.5, 0.5], [1.0, 1.0]],
    )


def test_fit_returns_self():
    scaler = MinMaxScaler()
    assert scaler.fit(np.array([[0.0], [1.0]])) is scaler


def test_output_is_float64():
    out = MinMaxScaler().fit_transform(np.array([[1], [2], [3]]))
    assert out.dtype == np.float64


def test_accepts_nested_lists():
    out = MinMaxScaler().fit_transform([[0.0], [1.0], [2.0]])
    np.testing.assert_allclose(out.ravel(), [0.0, 0.5, 1.0])


@pytest.mark.parametrize("X", [np.zeros(3), np.zeros((2, 2, 2))])
def test_non_2d_input_is_rejected(X):
    with pytest.raises(ValueError):
        MinMaxScaler().fit(X)


def test_repr_shows_feature_range():
    assert repr(MinMaxScaler((-1.0, 1.0))) == "MinMaxScaler(feature_range=(-1.0, 1.0))"
